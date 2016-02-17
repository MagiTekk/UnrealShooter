// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "UnrealShooterDataSingleton.h"


UUnrealShooterDataSingleton::UUnrealShooterDataSingleton()
{
	UUnrealShooterDataSingleton::InitSequenceData();
}

void UUnrealShooterDataSingleton::InitSequenceData()
{
	TArray<FTargetWave> Waves;
	TArray<FVector> customLocations;

	FVector Arr[] = { FVector{ 0.0f, 0.0f, 0.0f }, FVector{ 0.0f, 1.0f, 0.0f }, FVector{ 1.0f, 1.0f, 0.0f } };
	customLocations.Append(Arr, ARRAY_COUNT(Arr));

	FTargetWave Wave(FRotatableTargetProperties(FVector(1.0f, 0.0f, 0.0f), 1.0f, ETargetType::FalseTarget, customLocations));
	Waves.Add(Wave);

	//intitialize all data here, then create our library to access the singleton and from the level scrip actor instantiate the sequence!
	//also create the multicast to "signal" when a target has been destroyed, we need to know when a wave has finished

	//empty
	//customLocations.Empty();

	

	//Sequences.Add(properties);
}

