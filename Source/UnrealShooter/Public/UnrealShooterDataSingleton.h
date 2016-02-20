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
	void InitSequenceA();
	
public:

	UUnrealShooterDataSingleton();

	FText OutErrorMessage;
	int32 ErrorLineNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSequence")
	FTargetSequence SequenceA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSequence")
	FTargetSequence SequenceB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSequence")
	FTargetSequence SequenceC;

	void ParseJSON();
};
