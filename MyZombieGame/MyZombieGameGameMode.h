// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyZombieGameGameMode.generated.h"

UCLASS(minimalapi)
class AMyZombieGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyZombieGameGameMode();
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "GamePlay")
		int32 GetHardness();
	UFUNCTION(BlueprintCallable, Category = "GamePlay")
		void GrendansRein();
	UFUNCTION(BlueprintCallable, Category = "GamePlay")
		void WeaponRein();
	UFUNCTION(BlueprintCallable, Category = "GamePlay")
		float GetDuration();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> ShotGunBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> MiniGunBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> SMGBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> GrendaBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> MineBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> Produector1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> Produector2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> Produector3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> Produector4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> Produector5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> Produector6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> Produector7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> Produector8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 waves;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 cirlce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TArray<AActor*> ProduectorLoc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TArray<AActor*> WeaponBoxLoc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float Duration;
	FTimerHandle NextWave;


	
private:
	void NextWaveFunc();
	void SpwanZombies(AActor* Loc, TSubclassOf<AActor> Productor);
	
	TSubclassOf<AActor> GetAProducetorClass();
	AActor* GetALoc();
	AActor* GetALocForWeapon();
	TArray<AActor> Produetors;

};




