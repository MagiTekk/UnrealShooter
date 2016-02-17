// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UnrealShooterStructData.h"
#include "UnrealShooterDataSingleton.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALSHOOTER_API UUnrealShooterDataSingleton : public UObject
{
	GENERATED_BODY()

	void InitSequenceData();
	
public:

	UUnrealShooterDataSingleton();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSequence")
	TArray<FTargetSequence> Sequences;
	
};
