// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "RotatableTarget.h"
#include "TargetSequence.h"
#include "UnrealShooterDataSingleton.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALSHOOTER_API UUnrealShooterDataSingleton : public UObject
{
	GENERATED_BODY()

	void InitSequenceData();
	void InitSequenceA();

	void ParseJSON();
	void ParseSequences(const TArray<TSharedPtr<FJsonValue>> &SequencesJSON);
	void ParseWaves(const TArray<TSharedPtr<FJsonValue>> &WavesJSON);
	void ParseTargets(const TArray<TSharedPtr<FJsonValue>> &TargetsJSON);
	void ParseLocations(const TArray<TSharedPtr<FJsonValue>> &LocationsJSON);
	ETargetType GetEnumByString(FString const& inString);
	FTargetLocation GetTargetLocationByID(int32 locationID);
	
public:

	UUnrealShooterDataSingleton();

	FText OutErrorMessage;
	int32 ErrorLineNumber;

	TArray<UTargetSequence> Sequences;
	TArray<FTargetWave> Waves;
	TArray<FRotatableTargetProperties> Targets;
	TArray<FTargetLocation> Locations;
};
