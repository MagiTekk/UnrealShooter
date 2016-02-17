// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UnrealShooterEnumData.generated.h"

UENUM()
enum class ETargetType : uint8
{
	DefaultTarget,
	FalseTarget,
	MidTarget,
	LowTarget
};

UENUM()
enum class ECustomNotifyName : uint8
{
	PistolEquippedNotify,
	PistolUnequippedNotify,
	PistolReloadedNotify,
	StepSoundNotify
};

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API UUnrealShooterEnumData : public UObject
{
	GENERATED_BODY()
	
	
public:

	UUnrealShooterEnumData();
	
};
