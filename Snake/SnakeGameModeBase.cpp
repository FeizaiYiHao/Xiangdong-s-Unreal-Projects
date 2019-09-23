// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameModeBase.h"

void ASnakeGameModeBase::SetToPause()
{
	State = EGameState::EPasuing;
}

void ASnakeGameModeBase::SetToPlay()
{
	State = EGameState::EPlaying;
}

void ASnakeGameModeBase::SetToMenu()
{
	State = EGameState::EMenu;
}

bool ASnakeGameModeBase::IsPausing()
{
	return (State == EGameState::EPasuing);
}

bool ASnakeGameModeBase::IsPlaying()
{
	return (State == EGameState::EPlaying);
}

bool ASnakeGameModeBase::IsMenu()
{
	return (State==EGameState::EMenu);
}
