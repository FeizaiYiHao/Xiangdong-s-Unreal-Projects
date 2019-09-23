// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Zombies.generated.h"
enum class EZombieType : short {
	Type1X,
	Type3X,
	Type5X,
	Type9x,
};
UCLASS()
class MYZOMBIEGAME_API AZombies : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombies();
	UCapsuleComponent* MyCapsule;
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable, Category = "State")
		bool IfIsDead();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> Project;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AActor> ExploEff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* HitSound;
	UFUNCTION(BlueprintCallable, Category = "Blink")
		bool IsBlinking();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IfisBlinking;
	EZombieType ZombieType;
	FTimerHandle DeathHandle;
	bool isDead;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Death();
	void CheckType();
	void Die();

};
