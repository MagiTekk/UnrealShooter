// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "UnrealShooterDataSingleton.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"


UUnrealShooterDataSingleton::UUnrealShooterDataSingleton()
{
	UUnrealShooterDataSingleton::ParseJSON();
	//UUnrealShooterDataSingleton::InitSequenceData();
}

void UUnrealShooterDataSingleton::InitSequenceData()
{
	TArray<FTargetWave> Sequence;
	TArray<FRotatableTargetProperties> WaveData;
	TArray<FVector> customLocations;

	UUnrealShooterDataSingleton::InitSequenceA();

	/*FVector targetMovement_0[] = { FVector{ 0.0f, 0.0f, 0.0f }, FVector{ 0.0f, 1.0f, 0.0f }, FVector{ 1.0f, 1.0f, 0.0f } };
	customLocations.Append(targetMovement_0, ARRAY_COUNT(targetMovement_0));*/

	/*
	FRotatableTargetProperties wave1[] = {	};
	WaveData.Append(wave1, ARRAY_COUNT(wave1));
	*/

	//wave creation
	WaveData.Add(FRotatableTargetProperties(FVector(1.0f, 1.0f, 0.0f), 2.0f, ETargetType::MaleTarget));

	//add this wave to sequence
	Sequence.Add(FTargetWave(WaveData));


	WaveData.Empty();
	WaveData.Add(FRotatableTargetProperties(FVector(0.0f, 1.0f, 0.0f), 2.0f, ETargetType::MaleTarget));

	//add this wave to sequence
	Sequence.Add(FTargetWave(WaveData));


	//create my sequence out of all the waves
	FTargetSequence SequenceA(FName(TEXT("SequenceA")), Sequence);

	//intitialize all data here, then create our library to access the singleton and from the level scrip actor instantiate the sequence!
	//also create the multicast to "signal" when a target has been destroyed, we need to know when a wave has finished

	//empty
	//customLocations.Empty();

	

	//Sequences.Add(properties);
}

void UUnrealShooterDataSingleton::InitSequenceA()
{
	TArray<FTargetWave> Sequence;
	for (int i = 0; i < 11; i++)
	{
		TArray<FVector> customLocations;
		TArray<FRotatableTargetProperties> WaveData;

		WaveData.Add(FRotatableTargetProperties(FVector(1.0f, 1.0f, 0.0f), 2.0f, ETargetType::MaleTarget));

		//add this wave to sequence
		Sequence.Add(FTargetWave(WaveData));
	}
}

void UUnrealShooterDataSingleton::ParseJSON()
{
	FString JsonString;
	const FString fileName = "D:/Projects/UnrealProjects/UnrealShooter/Shared/UnrealShooterData.json";
	FFileHelper::LoadFileToString(JsonString, *fileName);
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		const TArray<TSharedPtr<FJsonValue>> sequences = JsonObject->GetArrayField(TEXT("sequences"));
		const TArray<TSharedPtr<FJsonValue>> waves = JsonObject->GetArrayField(TEXT("waves"));

		for (int32 Index = 0; Index != waves.Num(); ++Index)
		{
			UE_LOG(LogTemp, Warning, TEXT("Lo lograste no joda!: %d"), Index);
		}

		//iterate over the array to get every sequence node
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *sequencesNode);
	}
}