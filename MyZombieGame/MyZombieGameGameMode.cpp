// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyZombieGameGameMode.h"
#include "MyZombieGameCharacter.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Core\Public\Containers\Array.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Engine\Public\TimerManager.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Engine\Public\UnrealClient.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Engine\Classes\Engine\World.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Engine\Classes\Components\ActorComponent.h"
#include "UObject/ConstructorHelpers.h"

AMyZombieGameGameMode::AMyZombieGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMyZombieGameGameMode::BeginPlay()
{
	Super::BeginPlay();
	waves = 1;
	cirlce = 1;
	Duration = 10;
	NextWaveFunc();

}

int32 AMyZombieGameGameMode::GetHardness()
{
	return cirlce;
}

void AMyZombieGameGameMode::NextWaveFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("w %d c %d"),waves,cirlce);
	if (NextWave.IsValid()) {
		GetWorldTimerManager().ClearTimer(NextWave);
	}
	switch (waves)
	{
	case 1:
		SpwanZombies(GetALoc(), GetAProducetorClass());
		SpwanZombies(GetALoc(), GetAProducetorClass());
		break;
	case 2:
		SpwanZombies(GetALoc(), GetAProducetorClass());
		SpwanZombies(GetALoc(), GetAProducetorClass());
		SpwanZombies(GetALoc(), GetAProducetorClass());
		break;
	case 3 :
		SpwanZombies(GetALoc(), GetAProducetorClass());
		SpwanZombies(GetALoc(), GetAProducetorClass());
		SpwanZombies(GetALoc(), GetAProducetorClass());
		SpwanZombies(GetALoc(), GetAProducetorClass());
		break;
	case 4:
		SpwanZombies(GetALoc(), GetAProducetorClass());
		SpwanZombies(GetALoc(), GetAProducetorClass());
		SpwanZombies(GetALoc(), GetAProducetorClass());
		SpwanZombies(GetALoc(), GetAProducetorClass());
		SpwanZombies(GetALoc(), GetAProducetorClass());
		break;
	default:
		break;
	}
	waves++;
	
	if (waves == 5) {
		waves = 1;
		cirlce++;
		Duration += 10.0f;
	}
	GetWorldTimerManager().SetTimer(NextWave, this, &AMyZombieGameGameMode::NextWaveFunc, Duration, false);
}

void AMyZombieGameGameMode::SpwanZombies(AActor* Loc, TSubclassOf<AActor> Productor)
{
	UWorld* const World = GetWorld();
	FRotator SpawnRotation = Loc->GetActorRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	FVector SpawnLocation = Loc->GetActorLocation();
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
   AActor* NewPro= World->SpawnActor<AActor>(Productor, SpawnLocation, SpawnRotation, ActorSpawnParams);
   
}

void AMyZombieGameGameMode::GrendansRein()
{
	UWorld* const World = GetWorld();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	FVector SpawnLocation = GetALocForWeapon()->GetActorLocation();
	SpawnLocation.Z = 50.0f;
	SpawnLocation.X += FMath::RandRange(-200.0f, 200.0f);
	SpawnLocation.Y += FMath::RandRange(-200.0f, 200.0f);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	int32 Rand = FMath::RandRange(0, 1);
	TSubclassOf<AActor> Grendas = NULL;
	if (Rand == 0) {
		Grendas = GrendaBox;
	}
	else {
		Grendas = MineBox;
	}
	AActor* NewPro = World->SpawnActor<AActor>(Grendas, SpawnLocation, SpawnRotation, ActorSpawnParams);
}

void AMyZombieGameGameMode::WeaponRein()
{
	UWorld* const World = GetWorld();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	FVector SpawnLocation = GetALocForWeapon()->GetActorLocation();
	FActorSpawnParameters ActorSpawnParams;
	SpawnLocation.Z = 50.0f;
	SpawnLocation.X += FMath::RandRange(-200.0f, 200.0f);
	SpawnLocation.Y += FMath::RandRange(-200.0f, 200.0f);
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	int32 Rand = FMath::RandRange(0, 2);
	TSubclassOf<AActor> WeaponType = NULL;
	if (Rand == 0) {
		WeaponType = MiniGunBox;
	}
	else if(Rand==1){
		WeaponType = SMGBox;
	}
	else if (Rand == 2) {
		WeaponType = ShotGunBox;
	}
	AActor* NewPro = World->SpawnActor<AActor>(WeaponType, SpawnLocation, SpawnRotation, ActorSpawnParams);
}

float AMyZombieGameGameMode::GetDuration()
{
	return Duration;
}

TSubclassOf<AActor> AMyZombieGameGameMode::GetAProducetorClass()
{
	float max = 0.0f;
	if (cirlce > 8) {
		float max = 7.9f;
	}
	else {
		float max = cirlce;
	}
		
	int32 result = FMath::FloorToInt(FMath::RandRange(0.0f, max));
	switch (result)
	{
	case 0:
		return Produector1;
		break;
	case 1:
		return Produector2;
		break;
	case 2:
		return Produector3;
		break;
	case 3:
		return Produector4;
		break;
	case 4:
		return Produector5;
		break;
	case 5:
		return Produector6;
		break;
	case 6:
		return Produector7;
		break;
	case 7:
		return Produector8;
		break;
	default:
		return NULL;
		break;
	}
	
}

AActor* AMyZombieGameGameMode::GetALoc()
{
	return (ProduectorLoc[ FMath::RandRange(0, ProduectorLoc.Num()-1)]);
}

AActor * AMyZombieGameGameMode::GetALocForWeapon()
{
	return  (WeaponBoxLoc[FMath::RandRange(0, WeaponBoxLoc.Num() - 1)]);;
}
