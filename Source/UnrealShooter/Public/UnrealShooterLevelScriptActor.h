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

	UWorld* WorldReference;
	int32 Points;
	int32 ChainHits;
	int32 AmountNormalHits;
	int32 AmountHeadshotHits;
	int32 AmountBonusHits;
	FName CurrentSequenceName;
	FName GetSequenceEnumCodeName(ESequenceEnum sequenceType);

	/*
	* **If you don't specify this UPROPERTY then the GC will get rid of this UObject**
	* please remember this, also you could set this as a root object since we keep
	* it here on the level BP to make use of it all the time, this will make it as
	* an UObject that will never get GC
	* REF: https://answers.unrealengine.com/questions/2703/is-there-a-way-to-flag-a-uobject-to-never-be-garba.html
	*/
	UPROPERTY()
	UTargetSequence* CurrentSequence;

	UClass* CameraShakeBP;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// On remove from world
	virtual void BeginDestroy() override;

	UFUNCTION()
		void ResetTargetsHit();

	UFUNCTION()
		void OnTargetHit(bool bIsHeadshot, bool bIsBonusTarget);

	UFUNCTION()
		void CameraShake();

	UFUNCTION()
		void InitSequenceValues();

	UFUNCTION()
		void DisplayRewardedPoints(int32 points, FVector Location);

#pragma region Sequences

	UFUNCTION()
		void PlaySequence(ESequenceEnum sequenceType);

#pragma endregion
};
