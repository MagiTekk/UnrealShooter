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
		TArray<FTargetWave> Waves;

	FTargetSequenceStruct()
	{
	}

	FTargetSequenceStruct(FString sequenceName, TArray<FTargetWave> Waves)
	{
		this->sequenceName = sequenceName;
		this->Waves = Waves;
	}
};

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API UTargetSequence : public UObject
{
	GENERATED_BODY()

	FTargetWave _currentWave;
	TArray<ARotatableTarget*> _currentWaveTargets;
	int32 TargetsAvailable;
	FTargetWave GetNextWave();
	
public:

	UTargetSequence();

	FString sequenceName;
	TArray<FTargetWave> Waves;

	UWorld* World;
	UClass* TargetBP;
	//UClass* TargetCylinderBP;
	
	UFUNCTION()
	void OnTargetDestroyedHandler();

	void ApplyProperties(FString sequenceName, TArray<FTargetWave> Waves, UWorld* World);
	void PlayNextWave();
	
};
