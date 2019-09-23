// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyZombieGameCharacter.generated.h"
class USkeletalMeshComponent;
enum class EWeaponType : short {
	EHandgun,
	ESMG,
	EShotgun,
	EMiniGun,
};

enum class EBoomType : short {
	EMine,
	EGrenda,
	ENone,
};

UCLASS(config=Game)
class AMyZombieGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMyZombieGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* MeshComp;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseLookUpRate;
	class AWeapon* MyWeapon;
	EWeaponType CurrentWeapon;
	EBoomType CurrentBoom;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	/** Resets HMD orientation in VR. */
//	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
//	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
//	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	FTimerHandle TimerBeginPlay;
	void SetWeaponLoc();
	//void FireW(/*float Value*/);
	void StartFire();
	void EndFire();
	void OnFire();
	void CoolDownDone();
	//void FireWEnd();
	void TurnW();
	void TurnWRel();
	void TurnRel(char key);
	void TurnS();
	void TurnSRel();
	void TurnD();
	void TurnDRel();
	void TurnA();
	void TurnARel();
	void SetDir();
	void Grenade();
	void SetUnProtected();
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName Socket1Name;
	UFUNCTION(BlueprintCallable, Category = "Damage")
		int32 GetFiringState();
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ShotGun;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> Zombie;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> HugeZombie;
	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AActor> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AActor> GrenadeClass;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AActor> MineClass;
	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* ShotGunFireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* SMGFireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* HandGunFireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* MiniGunFireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* BoomFireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* ShotGunPickUpSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* SMGPickUpSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* HandGunPickUpSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* MiniGunPickUpSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* BoomPickUpSound;
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		uint32 bUsingMotionControllers : 1;
	/** Bullets Number */
	int32 Bullets;
	int32 Booms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool HitAndChangeTheMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<UCameraShake> MiniGunCamShake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<UCameraShake> ShotGunCamShake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<UCameraShake> SMGCamShake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<UCameraShake> HandGunCamShake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<UCameraShake> CamShake;
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
	/*Current Weapon Type*/


	/*Firing Rate*/

	float FiringRate;
	FRotator FaceDir;
	int32 DirKeyPressed;
	void Fire();
	void PlayFireSound();
	FTimerHandle FiringTimer;
	FTimerHandle HandgunTimer;
	FTimerHandle CamTimer;
	FTimerHandle UnProtectedTimer;
	bool OnCoolDown;
	bool Firing;
	FVector Dir;
	UCapsuleComponent* MyCapsule;
	float Health;
	bool isProtected;
	int32 IsUnderAttackBy;
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	void SetBackCam();
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

