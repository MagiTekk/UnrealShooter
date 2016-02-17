// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UnrealShooterEnumData.h"
#include "UnrealShooterStructData.generated.h"

USTRUCT()
struct FRotatableTargetProperties
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		FVector InitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		TArray<FVector> Locations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		float TimeToLive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		float Points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		float HeadshotPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		bool bIsExplosive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		ETargetType TargetType;

	//For GC
	void Destroy()
	{
	}

	FRotatableTargetProperties()
	{
		this->TimeToLive = 5.0f;
	}

	FRotatableTargetProperties(FVector InitialLocation, float TimeToLive, ETargetType TargetType = ETargetType::DefaultTarget, TArray<FVector> Locations = TArray<FVector>{}, float Speed = 1.0f, bool bIsExplosive = false)
	{
		this->InitialLocation = InitialLocation;
		this->Locations = Locations;
		this->Speed = Speed;
		this->TimeToLive = TimeToLive;
		this->bIsExplosive = bIsExplosive;
		this->TargetType = TargetType; //defines points rewarded per hit & target color
	}
};

USTRUCT()
struct FTargetWave
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	FRotatableTargetProperties TargetProperties;

	//For GC
	void Destroy()
	{
	}

	FTargetWave()
	{
	}

	FTargetWave(FRotatableTargetProperties TargetProperties)
	{
		this->TargetProperties = TargetProperties;
	}
};

USTRUCT()
struct FTargetSequence
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sequence)
		FName SequenceName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sequence)
		TArray<FTargetWave> Waves;

	//For GC
	void Destroy()
	{
	}

	FTargetSequence()
	{
	}

	FTargetSequence(FName SequenceName, TArray<FTargetWave> Waves)
	{
		this->SequenceName = SequenceName;
		this->Waves = Waves;
	}
};

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API UUnrealShooterStructData : public UObject
{
	GENERATED_BODY()
	
	
public:

	UUnrealShooterStructData();
	
};
