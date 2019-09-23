// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeHUD.h"
#include "C:\Program Files\Epic Games\UE_4.22\Engine\Source\Runtime\Engine\Classes\Kismet\GameplayStatics.h"
#include "C:\Users\16259\Documents\Unreal Projects\Snake\Source\Snake\SnakeGameModeBase.h"
#include "Engine/Canvas.h"

void ASnakeHUD::DrawHUD()
{
	class ASnakeGameModeBase* GameMode = Cast<ASnakeGameModeBase>(UGameplayStatics::GetGameMode(this));
	switch (GameMode->State)
	{
	case EGameState::EMenu:
		DrawText(TEXT("      SNAKE \n P to Play or Pasue \n  R to Restart\n  M to Menu"), FColor::Blue, (Canvas->SizeX / 2 - 100), (Canvas->SizeY / 2 - 100), HUDFont,3.0f);
		break;
	case EGameState::EPlaying:
		DrawText((TEXT("Score: ")+FString::FromInt(GameMode->score)), FColor::Blue, 20.0f, 20.f, HUDFont, 2.0f);
		break;
	case EGameState::EDead:
		DrawText(TEXT("   YOU ARE DEAD\n    M to Menu"), FColor::Blue, (Canvas->SizeX / 2 - 100), (Canvas->SizeY / 2 - 100), HUDFont, 3.0f);
		break;
	case EGameState::EPasuing:
		DrawText(TEXT("      Pasued \n P to Contiune Play"), FColor::Blue, (Canvas->SizeX / 2 - 100), (Canvas->SizeY / 2 - 100), HUDFont, 3.0f);
		break;
	default:
		break;
	}
}
