// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "RotatableTarget.h"
#include "TargetSequence.h"
#include "UnrealShooterDataSingleton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetDelegate_OnActorEndOverlap);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetDelegate_OnTargetDestroyed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetDelegate_OnSpecialTargetDestroyed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetDelegate_OnWeaponShot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetDelegate_OnWeaponReloaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetDelegate_OnWeaponEquipped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetDelegate_OnUISelection);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDelegate_OnUINavigation, FVector2D, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDelegate_OnActorBeginOverlap, AActor*, ContextActor);

UENUM()
enum class ESequenceEnum : uint8
{
	SequenceA,
	SequenceB
};

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
	void ParseTargetTypes(const TArray<TSharedPtr<FJsonValue>> &TargetTypesJSON);
	void ParseLocations(const TArray<TSharedPtr<FJsonValue>> &LocationsJSON);
	ETargetType GetEnumByString(FString const& inString);
	FTargetLocation GetTargetLocationByID(int32 locationID);
	FTargetWave GetTargetWaveByWaveID(int32 waveID);
	FRotatableTargetProperties GetTargetPropertiesByTargetID(int32 targetID);
	ETargetType GetTargetTypeByTargetID(int32 targetTypeID);
	int32 GetTargetPointsByTargetTypeID(int32 targetTypeID);
	int32 GetTargetHeadShotPointsByTargetTypeID(int32 targetTypeID);
	
public:

	UUnrealShooterDataSingleton();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Level Events")
		FTargetDelegate_OnActorBeginOverlap OnActorBeginOverlap;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Level Events")
		FTargetDelegate_OnActorEndOverlap OnActorEndOverlap;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Level Events")
		FTargetDelegate_OnTargetDestroyed OnTargetDestroyed;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Level Events")
		FTargetDelegate_OnSpecialTargetDestroyed OnSpecialTargetDestroyed;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Player Events")
		FTargetDelegate_OnWeaponShot OnWeaponShot;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Player Events")
		FTargetDelegate_OnWeaponReloaded OnWeaponReloaded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Player Events")
		FTargetDelegate_OnWeaponEquipped OnWeaponEquipped;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UI Events")
		FTargetDelegate_OnUINavigation OnUINavigation;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UI Events")
		FTargetDelegate_OnUISelection OnUISelection;

	TArray<FTargetSequenceStruct> Sequences;
	TArray<FTargetWave> Waves;
	TArray<FRotatableTargetProperties> Targets;
	TArray<FTargetLocation> Locations;
	TArray<FTargetTypeProperties> TargetTypes;

	FTargetSequenceStruct GetSequenceBySequenceName(FString SequenceName);
	FTargetSequenceStruct GetSequenceBySequenceEnum(ESequenceEnum SequenceEnum);
};
