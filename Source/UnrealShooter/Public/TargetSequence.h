// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "RotatableTarget.h"
#include "TargetSequence.generated.h"

USTRUCT()
struct FTargetWave
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		int32 WaveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		TArray<FRotatableTargetProperties> Targets;

	FTargetWave()
	{
	}

	FTargetWave(int32 WaveID, TArray<FRotatableTargetProperties> Targets)
	{
		this->WaveID = WaveID;
		this->Targets = Targets;
	}
};

USTRUCT()
struct FTargetSequenceStruct
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		FString sequenceName;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		TArray<FTargetWave> WaveIDs;

	FTargetSequenceStruct()
	{
	}

	FTargetSequenceStruct(FString sequenceName, TArray<FTargetWave> WaveIDs)
	{
		this->sequenceName = sequenceName;
		this->WaveIDs = WaveIDs;
	}
};

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API UTargetSequence : public UObject
{
	GENERATED_BODY()
	
public:

	UTargetSequence();

	FString sequenceName;
	TArray<FTargetWave> Waves;

	UClass* TargetBP;
	//UClass* TargetCylinderBP;
	
	void ApplyProperties(FString sequenceName, TArray<FTargetWave> Waves);
	void OnTargetDestroyedHandler();
	void Play();
	
};
