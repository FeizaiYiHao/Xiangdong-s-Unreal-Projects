// Fill out your copyright notice in the Description page of Project Settings.


#include "Body.h"
#include "C:\Users\16259\Documents\Unreal Projects\Snake\Source\Snake\SnakeHead.h"
#include "Runtime/Engine/Classes/Engine/World.h"
// Sets default values
ABody::ABody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABody::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ABody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//LifeSpan -= DeltaTime;
	//if (LifeSpan <= 0) {
	
		
		
	//	AActor::Destroy();
	//}
}

