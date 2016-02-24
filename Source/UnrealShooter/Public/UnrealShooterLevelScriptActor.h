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

	TArray<FRotatableTargetProperties> Sequence_0;
	
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

#pragma region Sequences

	void PlaySequence(int32 sequenceID=0);

#pragma endregion
};
