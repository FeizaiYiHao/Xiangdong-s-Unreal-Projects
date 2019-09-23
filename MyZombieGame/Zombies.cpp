// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Engine\Classes\GameFramework\Character.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Engine\Classes\Kismet\GameplayStatics.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Engine\Public\TimerManager.h"

// Sets default values
AZombies::AZombies()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AZombies::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(Project) && !isDead) {
		Health--;
		IfisBlinking = true;
		OtherActor->Destroy();
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
			if (Health <= 0) {
				Die();
				
			}
	}
	else if (OtherActor->IsA(ExploEff) && !isDead) {
		UE_LOG(LogTemp, Warning, TEXT("Grands"));
		Health--;
		Health--;
		Health--;
		IfisBlinking = true;
		if (Health <= 0) {
			Die();

		}
	}
}

bool AZombies::IfIsDead()
{
	return isDead;
}

bool AZombies::IsBlinking()
{
	return IfisBlinking;
}

// Called when the game starts or when spawned
void AZombies::BeginPlay()
{
	IfisBlinking = false;
	if (Health == 1) {
		ZombieType = EZombieType::Type1X;
	}
	if (Health == 3) {
		ZombieType = EZombieType::Type3X;
	}
	if (Health == 5) {
		ZombieType = EZombieType::Type5X;
	}
	if (Health == 9) {
		ZombieType = EZombieType::Type9x;
	}
	isDead = false;
	Super::BeginPlay();
	MyCapsule = GetCapsuleComponent();
	CheckType();
	if (MyCapsule ) {
		//UE_LOG(LogTemp, Warning, TEXT("Set"));
	}
	MyCapsule->OnComponentBeginOverlap.AddDynamic(this, &AZombies::OnOverlapBegin);
}

// Called every frame
void AZombies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombies::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombies::Death()
{
	Destroy();
}

void AZombies::CheckType()
{
	switch (ZombieType)
	{
	case EZombieType::Type1X:
		Health = 1;
		break;
	case EZombieType::Type3X:
		Health = 3;
		GetCharacterMovement()->MaxWalkSpeed = (GetCharacterMovement()->MaxWalkSpeed)*1.2;
		MyCapsule->RelativeScale3D = FVector(1.2f, 1.2f, 1.2f);
		break;
	case EZombieType::Type5X:
		Health = 5;
		GetCharacterMovement()->MaxWalkSpeed = (GetCharacterMovement()->MaxWalkSpeed)*1.4;
		MyCapsule->RelativeScale3D = FVector(1.4f, 1.4f, 1.4f);
		break;
	case EZombieType::Type9x:
		Health = 9;
		GetCharacterMovement()->MaxWalkSpeed = (GetCharacterMovement()->MaxWalkSpeed)*1.55;
		MyCapsule->RelativeScale3D = FVector(1.55f, 1.55f, 1.55f);
		break;
	default:
		break;
	}
}

void AZombies::Die()
{
	isDead = true;
	MyCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	MyCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	DetachFromControllerPendingDestroy();
	GetWorldTimerManager().SetTimer(DeathHandle, this, &AZombies::Death, 0.8f, false);
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
}



