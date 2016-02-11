// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "BasicSpawnPoint.h"


// Sets default values
ABasicSpawnPoint::ABasicSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
}

ABasicSpawnPoint::ABasicSpawnPoint(FVector spawnPosition)
{
	SpawnPosition = spawnPosition;
}


// Called when the game starts or when spawned
void ABasicSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*void ABasicSpawnPoint::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}*/

