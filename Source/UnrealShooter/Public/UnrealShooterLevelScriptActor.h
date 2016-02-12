// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LevelScriptActor.h"
#include "UnrealShooterLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API AUnrealShooterLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

	// Given the position in our grid, get the world location for the spawn
	FVector GetSpawnPoint(FVector SpawnPosition);
	
public:

	AUnrealShooterLevelScriptActor();

	// Custom name for each Level, specified in LevelBP's Default Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UnrealShooterLevel)
	FName UnrealShooterLevelName;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// On remove from world
	virtual void BeginDestroy() override;

	UClass* TargetBP;
	UClass* TargetCylinderBP;

#pragma region Sequences

	// Define each sequence of the level
	void InitiateSequence_0();

#pragma endregion
};
