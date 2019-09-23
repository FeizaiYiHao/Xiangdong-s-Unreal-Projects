// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyZombieGameCharacter.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Engine\Classes\GameFramework\Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Core\Public\Math\UnrealMathUtility.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Engine\Classes\Kismet\GameplayStatics.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Engine\Public\TimerManager.h"
#include "MyZombieGame\Weapon.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AMyZombieGameCharacter

AMyZombieGameCharacter::AMyZombieGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	MyCapsule = GetCapsuleComponent();
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	Socket1Name = "Socket1";
}

//////////////////////////////////////////////////////////////////////////
// Input



void AMyZombieGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("TurnW", IE_Pressed, this, &AMyZombieGameCharacter::TurnW);
	PlayerInputComponent->BindAction("TurnW", IE_Released, this, &AMyZombieGameCharacter::TurnWRel);
	PlayerInputComponent->BindAction("TurnD", IE_Pressed, this, &AMyZombieGameCharacter::TurnD);
	PlayerInputComponent->BindAction("TurnD", IE_Released, this, &AMyZombieGameCharacter::TurnDRel);
	PlayerInputComponent->BindAction("TurnA", IE_Pressed, this, &AMyZombieGameCharacter::TurnA);
	PlayerInputComponent->BindAction("TurnA", IE_Released, this, &AMyZombieGameCharacter::TurnARel);
	PlayerInputComponent->BindAction("TurnS", IE_Pressed, this, &AMyZombieGameCharacter::TurnS);
	PlayerInputComponent->BindAction("TurnS", IE_Released, this, &AMyZombieGameCharacter::TurnSRel);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyZombieGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyZombieGameCharacter::MoveRight);
	PlayerInputComponent->BindAction("Grenade", IE_Pressed, this, &AMyZombieGameCharacter::Grenade);
	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyZombieGameCharacter::OnResetVR);
}

void AMyZombieGameCharacter::SetBackCam()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
}


void AMyZombieGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	Bullets = 0;
	Health = 100;
	CurrentWeapon = EWeaponType::EHandgun;
	OnCoolDown = false;
	IsUnderAttackBy = 0;
	HitAndChangeTheMat = false;
	isProtected = false;
	// 0.4 0.66 0.8 0.13
	FiringRate = 0.4f;
	GetWorldTimerManager().SetTimer(TimerBeginPlay, this, &AMyZombieGameCharacter::SetWeaponLoc, 0.25f, false);
	MyCapsule = GetCapsuleComponent();
	if (MyCapsule) {
		//UE_LOG(LogTemp, Warning, TEXT("Set"));
	}
	MyCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMyZombieGameCharacter::OnOverlapBegin);
	MyCapsule->OnComponentEndOverlap.AddDynamic(this, &AMyZombieGameCharacter::OnOverlapEnd);
	Health = 100.0f;
	CurrentBoom = EBoomType::EMine;
	Booms = 5;
}

void AMyZombieGameCharacter::Tick(float DeltaTime)
{//UE_LOG(LogTemp, Warning, TEXT("Zombies %d"), IsUnderAttackBy);
	//UE_LOG(LogTemp, Warning, TEXT("X,%f Y,%f Z,%f"), Dir.X, Dir.Y,Dir.Z);
	if (!isProtected) {
		if (DeltaTime * 15.0f + Health >= 100) {
			Health = 100;
		}
		else
		{
			Health += DeltaTime * 25.0f;
		}
		
	}
	if (IsUnderAttackBy > 0) {
		
		if (!isProtected) {
			Health -= 20;
			HitAndChangeTheMat = true;
			isProtected = true;
			if (!UnProtectedTimer.IsValid()) {
				GetWorldTimerManager().SetTimer(UnProtectedTimer, this, &AMyZombieGameCharacter::SetUnProtected, 0.6f, false);
			}
		}
	}
	
}



void AMyZombieGameCharacter::SetWeaponLoc()

{
	GetWorld()->SpawnActor<AActor>(Zombie, FVector::ZeroVector, FRotator::ZeroRotator);
	FVector Current = GetCapsuleComponent()->GetComponentLocation();
	FAttachmentTransformRules Rules = (FAttachmentTransformRules::KeepRelativeTransform, FAttachmentTransformRules::KeepRelativeTransform, FAttachmentTransformRules::KeepRelativeTransform);
	AWeapon* My = GetWorld()->SpawnActor<AWeapon>(ShotGun, FVector::ZeroVector, FRotator::ZeroRotator);

	MyWeapon = My;
	MyWeapon->AttachToComponent(Cast<USceneComponent>(GetMesh()), Rules, Socket1Name);
	//MyWeapon->SetActorLocation(FVector::ZeroVector);

}



void AMyZombieGameCharacter::StartFire()
{
	Firing = true;
	if (!FiringTimer.IsValid()) {
		OnFire();
	}
	else {

	}
}



void AMyZombieGameCharacter::EndFire()
{
	Firing = false;
}

void AMyZombieGameCharacter::OnFire()
{

	if (Firing) {
		Fire();

		// try and play the sound if specified
		PlayFireSound();
		

		// try and play a firing animation if specified
		/*if (FireAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}*/
		if (Bullets == 0) {
			FiringRate = 0.4f;
			CurrentWeapon = EWeaponType::EHandgun;
		}
		GetWorldTimerManager().SetTimer(FiringTimer, this, &AMyZombieGameCharacter::OnFire, FiringRate, false);
	}
	else {
		GetWorldTimerManager().ClearTimer(FiringTimer);
	}
}

void AMyZombieGameCharacter::Fire()
{
	UWorld* const World = GetWorld();
	FRotator SpawnRotation = GetActorRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	FVector SpawnLocation = GetActorLocation();

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	SpawnLocation = SpawnLocation+ (Dir * 50.0f);
	// spawn the projectile at the muzzle
	if (CurrentWeapon == EWeaponType::EMiniGun) {
		SpawnRotation.Yaw += FMath::RandRange(1.5f, -1.5f);
		SpawnLocation.X += FMath::RandRange(10.0f, -10.0f);
		SpawnLocation.Y += FMath::RandRange(10.0f, -10.0f);
	}
	World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	if (CurrentWeapon == EWeaponType::EShotgun) {
		SpawnRotation.Yaw += 3.0f;
		World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		SpawnRotation.Yaw += 3.0f;
		World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		SpawnRotation.Yaw += 3.0f;
		World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		SpawnRotation.Yaw -= 9.0f;
		SpawnRotation.Yaw -= 3.0f;
		World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		SpawnRotation.Yaw -= 3.0f;
		World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		SpawnRotation.Yaw -= 3.0f;
		World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}	
	APlayerController* PC = Cast<APlayerController>(GetController());
	switch (CurrentWeapon)
	{
	case EWeaponType::EHandgun:
	
		PC->ClientPlayCameraShake(HandGunCamShake);
		break;
	case EWeaponType::EMiniGun:
		
		PC->ClientPlayCameraShake(MiniGunCamShake);
		break;
	case EWeaponType::EShotgun:
	
		PC->ClientPlayCameraShake(ShotGunCamShake);
		break;
	case EWeaponType::ESMG:

		PC->ClientPlayCameraShake(SMGCamShake);
		break;
	default:
		break;
	}
	Bullets--;
}
void AMyZombieGameCharacter::PlayFireSound()
{
	switch (CurrentWeapon)
	{
	case EWeaponType::EHandgun:
		UGameplayStatics::PlaySoundAtLocation(this, HandGunFireSound, GetActorLocation());
		break;
	case EWeaponType::EMiniGun:
		UGameplayStatics::PlaySoundAtLocation(this, MiniGunFireSound, GetActorLocation());
		break;
	case EWeaponType::EShotgun:
		UGameplayStatics::PlaySoundAtLocation(this, ShotGunFireSound, GetActorLocation());
		break;
	case EWeaponType::ESMG:
		UGameplayStatics::PlaySoundAtLocation(this, SMGFireSound, GetActorLocation());
		break;
	default:
		break;
	}
}
void AMyZombieGameCharacter::CoolDownDone()
{
	OnCoolDown = false;
}

void AMyZombieGameCharacter::TurnW()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	if (CamTimer.IsValid()) {
		GetWorldTimerManager().ClearTimer(CamTimer);
	}	
	DirKeyPressed++;
	//FVector Result = FVector::ZeroVector;
	Dir.X += 1.0f;
	//Dir += Result;


	
	SetActorRotation(Dir.ToOrientationRotator());
		StartFire();
}

void AMyZombieGameCharacter::TurnWRel()
{
	TurnRel('W');
}

void AMyZombieGameCharacter::TurnDRel()
{
	TurnRel('D');
}



void AMyZombieGameCharacter::TurnRel(char key )
{
	DirKeyPressed--;

	if (DirKeyPressed == 0) {
		EndFire();
		Dir = FVector::ZeroVector;
		GetWorldTimerManager().SetTimer(CamTimer, this, &AMyZombieGameCharacter::SetBackCam, 0.5f, false);
	}
	else if (DirKeyPressed > 0) {
		FVector Result = FVector::ZeroVector;
		switch (key)
		{
		case 'W':
			Dir.X -= 1.0f;
			break;
		case 'S':
			Dir.X -= -1.0f;
			break;
		case 'A':
			Dir.Y -= -1.0f;
			break;
		case 'D':
			Dir.Y -= 1.0f;
			break;
		default:
			break;
		}
		
		SetActorRotation(Dir.ToOrientationRotator());
		
	}
}

void AMyZombieGameCharacter::TurnS()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	if (CamTimer.IsValid()) {
		GetWorldTimerManager().ClearTimer(CamTimer);
	}
	DirKeyPressed++;
	//FVector Result = FVector::ZeroVector;
	Dir.X += -1.0f;
	//Dir += Result;



	SetActorRotation(Dir.ToOrientationRotator());
	StartFire();
}

void AMyZombieGameCharacter::TurnSRel()
{
	TurnRel('S');
}

void AMyZombieGameCharacter::TurnD()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	if (CamTimer.IsValid()) {
		GetWorldTimerManager().ClearTimer(CamTimer);
	}
	//FVector Result = FVector::ZeroVector;
	Dir.Y += 1.0f;
	//Dir += Result;



	SetActorRotation(Dir.ToOrientationRotator());
	StartFire();
	DirKeyPressed++;
}

void AMyZombieGameCharacter::TurnA()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	if (CamTimer.IsValid()) {
		GetWorldTimerManager().ClearTimer(CamTimer);
	}
	DirKeyPressed++;
	//FVector Result = FVector::ZeroVector;
	Dir.Y += -1.0f;
	//Dir += Result;



	SetActorRotation(Dir.ToOrientationRotator());
	StartFire();
}

void AMyZombieGameCharacter::TurnARel()
{
	TurnRel('A');
}

void AMyZombieGameCharacter::SetDir()
{
	FRotator Dir = FRotator::ZeroRotator;

		

	switch (DirKeyPressed)
		{
		case 'W':
			Dir.Yaw = 0.0f;
			break;
		case 'S':
			Dir.Yaw = 180.0f;
			break;
		case 'A':
			Dir.Yaw = 270.0f;
			break;
		case 'D':
			Dir.Yaw = 90.0f;
			break;
		default:
			break;
	}
	

	SetActorRotation(Dir);
}

void AMyZombieGameCharacter::Grenade()
{
	if (CurrentBoom == EBoomType::EMine) {
		UWorld* const World = GetWorld();
		FRotator SpawnRotation = GetActorRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		FVector SpawnLocation = GetActorLocation();
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		UGameplayStatics::PlaySoundAtLocation(this, BoomFireSound, GetActorLocation());
		SpawnLocation.Z = 50.0f;
		World->SpawnActor<AActor>(MineClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		Booms--;
	}
	else if (CurrentBoom == EBoomType::EGrenda) {
			if (Firing) {
				UWorld* const World = GetWorld();
				FRotator SpawnRotation = GetActorRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				FVector SpawnLocation = GetActorLocation();

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;

				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				SpawnLocation = SpawnLocation + (Dir * 70.0f);
				SpawnLocation.Z -= 20.0f;
				World->SpawnActor<AActor>(GrenadeClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				UGameplayStatics::PlaySoundAtLocation(this, BoomFireSound, GetActorLocation());
				Booms--;
			}
		}
	if (Booms <= 0) {
		CurrentBoom = EBoomType::ENone;
	}
}

void AMyZombieGameCharacter::SetUnProtected()
{
	isProtected = false;
	GetWorldTimerManager().ClearTimer(UnProtectedTimer);
}


void AMyZombieGameCharacter::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Health %f"),Health);
	if (OtherActor->IsA(ShotGunBox)) {
		OtherActor->Destroy();
		Bullets = 30;
		CurrentWeapon = EWeaponType::EShotgun;
		FiringRate = 0.8f;
		UGameplayStatics::PlaySoundAtLocation(this, ShotGunPickUpSound, GetActorLocation());
		if (FiringTimer.IsValid()) {
			GetWorldTimerManager().ClearTimer(FiringTimer);
			GetWorldTimerManager().SetTimer(FiringTimer, this, &AMyZombieGameCharacter::OnFire, 0.3f, false);
			
		}
	}
	if (OtherActor->IsA(MiniGunBox)) {
		OtherActor->Destroy();
		Bullets = 300;
		CurrentWeapon = EWeaponType::EMiniGun;
		FiringRate = 0.066f;
		UGameplayStatics::PlaySoundAtLocation(this, MiniGunPickUpSound, GetActorLocation());
		if (FiringTimer.IsValid()) {
			GetWorldTimerManager().ClearTimer(FiringTimer);
			GetWorldTimerManager().SetTimer(FiringTimer, this, &AMyZombieGameCharacter::OnFire, 0.3f, false);

		}
	}
	if (OtherActor->IsA(SMGBox)) {
		OtherActor->Destroy();
		Bullets = 150;
		CurrentWeapon = EWeaponType::ESMG;
		FiringRate = 0.13f;
		UGameplayStatics::PlaySoundAtLocation(this, SMGPickUpSound, GetActorLocation());
		if (FiringTimer.IsValid()) {
			GetWorldTimerManager().ClearTimer(FiringTimer);
			GetWorldTimerManager().SetTimer(FiringTimer, this, &AMyZombieGameCharacter::OnFire, 0.3f, false);

		}
	}
	if (OtherActor->IsA(Zombie)) {
		IsUnderAttackBy++;

	}
	if (OtherActor->IsA(GrendaBox)) {
		OtherActor->Destroy();
		UGameplayStatics::PlaySoundAtLocation(this, BoomPickUpSound, GetActorLocation());
		CurrentBoom = EBoomType::EGrenda;
		Booms = 7;

	}
	if (OtherActor->IsA(MineBox)) {
		OtherActor->Destroy();
		UGameplayStatics::PlaySoundAtLocation(this, BoomPickUpSound, GetActorLocation());
		CurrentBoom = EBoomType::EMine;
		Booms = 7;

	}
}
	
	


void AMyZombieGameCharacter::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(Zombie)) {
		IsUnderAttackBy--;
	}
}

int32 AMyZombieGameCharacter::GetFiringState()
{


	if (FiringTimer.IsValid()) {
		switch (CurrentWeapon)
		{
		case EWeaponType::EHandgun:
			return 1;
			break;
		case EWeaponType::ESMG:
			return 2;
			break;
		case EWeaponType::EShotgun:
			return 3;
			break;
		case EWeaponType::EMiniGun:
			return 4;
			break;
		default:
			return NULL;
			break;
		}
	}
	else { 
		return 0; 
	}
}

/*void AMyZombieGameCharacter::TurnW(float Value)
{
	FRotator Dir = FRotator::ZeroRotator;
	Dir.Yaw = (Value*90.0f);
	SetActorRotation(Dir);
}*/

void AMyZombieGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyZombieGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyZombieGameCharacter::MoveForward(float Value)
{
	FVector Dir = FVector::ZeroVector;
	Dir.X += Value;
	AddMovementInput(Dir);
	/*if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}*/
}

void AMyZombieGameCharacter::MoveRight(float Value)
{
	FVector Dir = FVector::ZeroVector;
	Dir.Y += Value;
	AddMovementInput(Dir);
	/*if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}*/
}
