// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SnakeGameModeBase.generated.h"

/**
 * 
 */
enum class EGameState : short {
	EPlaying,
	EPasuing,
	EMenu,
	EDead,

};

UCLASS()
class SNAKE_API ASnakeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:
	
	EGameState State = EGameState::EMenu;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ClassName;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ClassName1;
	UPROPERTY(EditAnywhere)
		float DefaultRefrashTime;
	void SetToPause();
	void SetToPlay();
	void SetToMenu();
	bool IsPausing();
	bool IsPlaying();
	bool IsMenu();
	
	int32 score;
};
