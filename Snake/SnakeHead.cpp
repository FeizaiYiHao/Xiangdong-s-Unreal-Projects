// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeHead.h"
#include "Components/InputComponent.h"
#include "GameFramework/Pawn.h"
#include "Snake\Body.h"
#include "Snake\SnakeGameModeBase.h"
#include "Engine\Source\Runtime\Engine\Classes\Components\ActorComponent.h"
#include "GameFramework\Character.h"
#include "Runtime\Engine\Public\UnrealClient.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime\Engine\Classes\Kismet\GameplayStatics.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ASnakeHead::ASnakeHead()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultRefrashTime = 0.5f;
	length = 6;
	MyCapsule = GetCapsuleComponent();

}

// Called when the game starts or when spawned
void ASnakeHead::BeginPlay()
{	Super::BeginPlay();
	
	GameMode = Cast<ASnakeGameModeBase>(UGameplayStatics::GetGameMode(this));
	UE_LOG(LogTemp, Warning, TEXT("Score %d"),GameMode->score);
	GameMode->SetToMenu();
	if (GameMode->DefaultRefrashTime != 0) {
		DefaultRefrashTime = GameMode->DefaultRefrashTime;
	}
	CurrentDir = Edirtion::EW;
	RefrashTime = DefaultRefrashTime;
	AFood* NewActor = GetWorld()->SpawnActor<AFood>(GameMode->ClassName1, GetAGoodLocation(), FRotator::ZeroRotator);
	Food = NewActor;
	PressTimer = 0.1;
	MyCapsule->OnComponentBeginOverlap.AddDynamic(this, &ASnakeHead::OnOverlapBegin);

}

// Called every frame
void ASnakeHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (GameMode->State == EGameState::EPlaying)

	//{
		PressTimer -= DeltaTime;
		RefrashTime -= DeltaTime;
	//}
	
	if (PressTimer <= 0) {
		CheckPress();
		PressTimer = 0.1;
	}
	
	if (RefrashTime <= 0) {
		play();
		
	}
	
}

void ASnakeHead::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("InputW", IE_Pressed, this, &ASnakeHead::TurnWPressed);
	InputComponent->BindAction("InputW", IE_Released, this, &ASnakeHead::TurnWReleased);
	InputComponent->BindAction("InputA", IE_Pressed, this, &ASnakeHead::TurnAPressed);
	InputComponent->BindAction("InputA", IE_Released, this, &ASnakeHead::TurnAReleased);
	InputComponent->BindAction("InputS", IE_Pressed, this, &ASnakeHead::TurnSPressed);
	InputComponent->BindAction("InputS", IE_Released, this, &ASnakeHead::TurnSReleased);
	InputComponent->BindAction("InputD", IE_Pressed, this, &ASnakeHead::TurnDPressed);
	InputComponent->BindAction("InputD", IE_Released, this, &ASnakeHead::TurnDReleased);
	InputComponent->BindAction("InputP", IE_Released, this, &ASnakeHead::Pause);
	InputComponent->BindAction("InputM", IE_Released, this, &ASnakeHead::Menu);
	InputComponent->BindAction("InputR", IE_Released, this, &ASnakeHead::restart);

}



void ASnakeHead::play()
{
	if (GameMode->IsPlaying()) {
		//UE_LOG(LogTemp, Warning, TEXT("%f"), RefrashTime);
		FVector Current = GetActorLocation();
		ABody* NewActor = GetWorld()->SpawnActor<ABody>(GameMode->ClassName, Current, FRotator::ZeroRotator);
		NewActor->LifeSpan = length;

		switch (CurrentDir)
		{
		case Edirtion::EW:
			Current.X += 10.0f;
			break;
		case Edirtion::ES:
			Current.X -= 10.0f;
			break;
		case Edirtion::ED:
			Current.Y += 10.0f;
			break;
		case Edirtion::EA:
			Current.Y -= 10.0f;
			break;
		default:
			break;

		}
		Current = SetHead(Current);
		//SetActorLocation(Current,false);

		CountingBodiesAndFood(Current);
		CountingDeadBodies();
		//	ABody* NewActor = GetWorld()->SpawnActor<ABody>(GetClass(), Current, FRotator::ZeroRotator);
		
	}
	RefrashTime = DefaultRefrashTime;
}

void ASnakeHead::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!OtherActor->IsA(GameMode->ClassName)) {
		UE_LOG(LogTemp, Warning, TEXT("OverLap"));
	}
	
}




//void ASnakeHead::rotat(Edirtion Dir)
//
//}

void ASnakeHead::TurnW()
{	
	if (CurrentDir == Edirtion::ES) {
		
	}
	else
	{
		CurrentDir = Edirtion::EW;
		

		play();
	}
}

void ASnakeHead::TurnS()
{
	if (CurrentDir == Edirtion::EW) {

	}
	else
	{
		CurrentDir = Edirtion::ES;
		

		play();
	}
}

void ASnakeHead::TurnA()
{
	if (CurrentDir == Edirtion::ED) {

	}
	else
	{
		CurrentDir = Edirtion::EA;

		play();
		
	}
}

void ASnakeHead::TurnD()
{
	if (CurrentDir == Edirtion::EA) {

	}
	else
	{
		CurrentDir = Edirtion::ED;
		

		play();
	}
}

void ASnakeHead::CountingBodiesAndFood(FVector Current)
{

	FVector FoodVec = Food->GetActorLocation();
		if (Current.X == FoodVec.X && Current.Y == FoodVec.Y) {
		//	UE_LOG(LogTemp, Warning, TEXT("EAT"));
			Food->Destroy();
			Food = nullptr;
			length++;
			GameMode->score++;
			PlusTime();
			AFood* NewActor = GetWorld()->SpawnActor<AFood>(GameMode->ClassName1, GetAGoodLocation(), FRotator::ZeroRotator);
		//	UE_LOG(LogTemp, Warning, TEXT("Food"));
			Food = NewActor;

		}
		for (TActorIterator<ABody> It(GetWorld()); It; ++It)
		{
			It->LifeSpan--;
			if (Current.X == It->GetActorLocation().X && Current.Y == It->GetActorLocation().Y &&It->LifeSpan!=0) {
				//UE_LOG(LogTemp, Warning, TEXT("Dead"));
				GameMode->State = EGameState::EDead;
			}
		}

}

void ASnakeHead::CountingDeadBodies()
{
	for (TActorIterator<ABody> It(GetWorld()); It; ++It)
	{

		if (It->LifeSpan <= 0) {
			It->Destroy();
		};

	}
}

FVector ASnakeHead::SetHead(FVector Current)
{
	if (Current.X == -500) {
		Current.X = 490;
	}
	else if (Current.X == 500) {
		Current.X = -490;
	}
	if (Current.Y == -740) {
		Current.Y = 730;
	}
	else if (Current.Y == 740) {
		Current.Y = -730;
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("%f %f"), Current.X, Current.Y);
	SetActorLocation(Current, false);
	return Current;
}

void ASnakeHead::TurnWPressed()
{
	WPressed = true;
}

void ASnakeHead::TurnWReleased()
{
	if (WPressedTime < 3) {
		TurnW();
	}
	WPressed = false;
	WPressedTime = 0;
}

void ASnakeHead::TurnAPressed()
{
	APressed = true;
}

void ASnakeHead::TurnAReleased()
{
	if (APressedTime < 3) {
		TurnA();
	}
	APressed = false;
	APressedTime = 0;
}

void ASnakeHead::TurnSPressed()
{
	SPressed = true;
}

void ASnakeHead::TurnSReleased()
{
	if (SPressedTime < 3) {
		TurnS();
	}
	SPressed = false;
	SPressedTime = 0;
}


void ASnakeHead::TurnDPressed()
{
	DPressed = true;
}

void ASnakeHead::TurnDReleased()
{
	if (DPressedTime < 3) {
		TurnD();
	}
	DPressed = false;
	DPressedTime = 0;
}

void ASnakeHead::CheckPress()
{
	if (WPressed) {
		WPressedTime++;
		if (WPressedTime >= 3) {
			TurnW();
		}
	}
	if (APressed) {
		APressedTime++;
		if (APressedTime >= 3) {
			TurnA();
		}
	}
	if (SPressed) {
		SPressedTime++;
		if (SPressedTime >= 3) {
			TurnS();
		}
	}
	if (DPressed) {
		DPressedTime++;
		if (DPressedTime >= 3) {
			TurnD();
		}
	}
}

void ASnakeHead::MinTime()
{
	for (TActorIterator<ABody> It(GetWorld()); It; ++It)
	{

		It->LifeSpan -= 1;

	}

}

void ASnakeHead::PlusTime()
{
	for (TActorIterator<ABody> It(GetWorld()); It; ++It)
	{
		
		It->LifeSpan += 1;
		
	}
}

void ASnakeHead::Pause()
{
	if (GameMode->State==EGameState::EMenu || GameMode->State==EGameState::EPasuing) {
		GameMode->SetToPlay();
		UE_LOG(LogTemp, Warning, TEXT("Set to Play"));
	}
	else if (GameMode->IsPlaying()) {
		GameMode->SetToPause();
	}
}

void ASnakeHead::Menu()
{
	GameMode->State = EGameState::EMenu;
}

void ASnakeHead::restart()
{
	UE_LOG(LogTemp, Warning, TEXT("R"));
	if (GameMode->State == EGameState::EMenu) {
		UE_LOG(LogTemp, Warning, TEXT("R"));
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
		GameMode->SetToPlay();
	}
}





FVector ASnakeHead::GetAGoodLocation()
{
	bool didntgetit = true;
	FVector ret= FVector::ZeroVector;
	ret.Z = GetActorLocation().Z;
	TArray<FVector> Vectors;
	Vectors.Add(GetActorLocation());

	for (TActorIterator<ABody> It(GetWorld()); It; ++It)
	{
		Vectors.Add(It->GetActorLocation());
	}
	while (didntgetit) {
		ret.X = 0;
		ret.Y = 0;
		ret.X += (FMath:: RandRange(-49, +49))*10;
		ret.Y += (FMath::RandRange(-74, +74)) * 10;
		for (FVector vec : Vectors) {
			if (vec == ret) {
				break;
			}
		}
		didntgetit = false;
	}
	UE_LOG(LogTemp, Warning, TEXT("%f %f %f"),ret.X,ret.Y,ret.Z);
	return ret;
}






