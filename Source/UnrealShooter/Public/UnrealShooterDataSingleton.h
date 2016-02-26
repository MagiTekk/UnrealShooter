// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "RotatableTarget.h"
#include "TargetSequence.h"
#include "UnrealShooterDataSingleton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetDelegate_OnTargetDestroyed);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALSHOOTER_API UUnrealShooterDataSingleton : public UObject
{
	GENERATED_BODY()

	void ParseJSON();
	void ParseSequences(const TArray<TSharedPtr<FJsonValue>> &SequencesJSON);
	void ParseWaves(const TArray<TSharedPtr<FJsonValue>> &WavesJSON);
	void ParseTargets(const TArray<TSharedPtr<FJsonValue>> &TargetsJSON);
	void ParseLocations(const TArray<TSharedPtr<FJsonValue>> &LocationsJSON);
	ETargetType GetEnumByString(FString const& inString);
	FTargetLocation GetTargetLocationByID(int32 locationID);
	FTargetWave GetTargetWaveByWaveID(int32 waveID);
	FRotatableTargetProperties GetTargetPropertiesByTargetID(int32 targetID);
	
	
public:

	UUnrealShooterDataSingleton();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Level Events")
		FTargetDelegate_OnTargetDestroyed OnTargetDestroyed;

	TArray<FTargetSequenceStruct> Sequences;
	TArray<FTargetWave> Waves;
	TArray<FRotatableTargetProperties> Targets;
	TArray<FTargetLocation> Locations;

	FTargetSequenceStruct GetSequenceBySequenceName(FString SequenceName);
};
