// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BasicSpawnPoint.generated.h"

UCLASS()
class UNREALSHOOTER_API ABasicSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicSpawnPoint();
	ABasicSpawnPoint(FVector spawnPosition);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	//virtual void Tick( float DeltaSeconds ) override;

	// This property is here to understand where this spawn is located within the grid
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Spawn)
	FVector SpawnPosition;
	
};
