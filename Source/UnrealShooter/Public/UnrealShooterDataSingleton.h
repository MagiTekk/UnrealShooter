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
	
public:

	UUnrealShooterDataSingleton();

	FText OutErrorMessage;
	int32 ErrorLineNumber;

	TArray<UTargetSequence> Sequences;
	TArray<FTargetWave> Waves;
	TArray<ARotatableTarget> Targets;
	TArray<FVector> Locations;

	void ParseJSON();
};
