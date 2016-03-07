// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LevelScriptActor.h"
#include "TargetSequence.h"
#include "UnrealShooterDataSingleton.h"
#include "UnrealShooterLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API AUnrealShooterLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:

	AUnrealShooterLevelScriptActor();

	// Custom name for each Level, specified in LevelBP's Default Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UnrealShooterLevel)
		FName UnrealShooterLevelName;

	/*
	* **If you don't specify this UPROPERTY then the GC will get rid of this UObject**
	* please remember this, also you could set this as a root object since we keep
	* it here on the level BP to make use of it all the time, this will make it as
	* an UObject that will never get GC
	* REF: https://answers.unrealengine.com/questions/2703/is-there-a-way-to-flag-a-uobject-to-never-be-garba.html
	*/
	UPROPERTY()
	UTargetSequence* CurrentSequence;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// On remove from world
	virtual void BeginDestroy() override;

#pragma region Sequences

	void PlaySequence(ESequenceEnum sequenceType);

#pragma endregion
};
