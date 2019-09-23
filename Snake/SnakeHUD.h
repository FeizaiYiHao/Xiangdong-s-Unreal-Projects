// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SnakeHUD.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE_API ASnakeHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite,Category=HUDFont)
		UFont * HUDFont;
	void DrawHUD() override;
};
