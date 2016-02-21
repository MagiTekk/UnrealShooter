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
		TArray<FRotatableTargetProperties> Wave;

	void SetWave(TArray<FRotatableTargetProperties> Wave)
	{
		this->Wave = Wave;
	}

	//For GC
	void Destroy()
	{
	}

	FTargetWave()
	{
	}

	FTargetWave(TArray<FRotatableTargetProperties> Wave)
	{
		this->Wave = Wave;
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
	
};
