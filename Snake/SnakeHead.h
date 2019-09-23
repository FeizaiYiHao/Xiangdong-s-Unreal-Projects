// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Body.h"
#include "GameFramework/Character.h"
#include "Runtime/Core/Public/Containers/Queue.h"
#include "Food.h"
#include "Engine\Classes\GameFramework\Character.h"
#include "SnakeHead.generated.h"

enum class Edirtion : short {
	EW,
	ES,
	EA,
	ED,
};

UCLASS()
class SNAKE_API ASnakeHead : public ACharacter
{
	
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASnakeHead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// ???????????
	/*UFUNCTION()
		void MoveForward(float Value);

	// ??????????
	UFUNCTION()
		void MoveRight(float Value);*/

public :

	UPROPERTY(EditAnywhere)
	float DefaultRefrashTime;
	UPROPERTY(EditAnywhere)
		AFood* Food;
	UFUNCTION()
	void play();
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UCapsuleComponent* MyCapsule;
	float RefrashTime;
	float Timer;
	UPROPERTY(EditAnywhere)
	int32 length;
	void TurnW();
	void TurnS();
	void TurnA();
	void TurnD();	
	void CountingBodiesAndFood(FVector Current);
	void CountingDeadBodies();
	FVector SetHead(FVector Current);
	void TurnWPressed();
	void TurnWReleased();
	void TurnAPressed();
	void TurnAReleased();
	void TurnSPressed();
	void TurnSReleased();
	void TurnDPressed();
	void TurnDReleased();
	bool WPressed;
	int32 WPressedTime;
	bool SPressed;
	int32 SPressedTime;
	bool APressed;
	int32 APressedTime;
	bool DPressed;
	int32 DPressedTime;
	float PressTimer;
	void CheckPress();
	void MinTime();
	void PlusTime();
	void Pause();
	void Menu();
	void restart();
	FVector GetAGoodLocation();



	class ASnakeGameModeBase* GameMode;
	//void Collsion(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	Edirtion CurrentDir;
};
