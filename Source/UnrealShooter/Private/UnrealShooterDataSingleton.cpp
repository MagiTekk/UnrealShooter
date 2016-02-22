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
	/*
	TArray<FTargetWave> Sequence;
	TArray<FRotatableTargetProperties> WaveData;
	TArray<FVector> customLocations;

	UUnrealShooterDataSingleton::InitSequenceA();
	*/

	/*FVector targetMovement_0[] = { FVector{ 0.0f, 0.0f, 0.0f }, FVector{ 0.0f, 1.0f, 0.0f }, FVector{ 1.0f, 1.0f, 0.0f } };
	customLocations.Append(targetMovement_0, ARRAY_COUNT(targetMovement_0));*/

	/*
	FRotatableTargetProperties wave1[] = {	};
	WaveData.Append(wave1, ARRAY_COUNT(wave1));
	*/

	//wave creation
	/*WaveData.Add(FRotatableTargetProperties(FVector(1.0f, 1.0f, 0.0f), 2.0f, ETargetType::MaleTarget));

	//add this wave to sequence
	Sequence.Add(FTargetWave(WaveData));


	WaveData.Empty();
	WaveData.Add(FRotatableTargetProperties(FVector(0.0f, 1.0f, 0.0f), 2.0f, ETargetType::MaleTarget));

	//add this wave to sequence
	Sequence.Add(FTargetWave(WaveData));


	//create my sequence out of all the waves
	FTargetSequence SequenceA(FName(TEXT("SequenceA")), Sequence);
	*/

	//intitialize all data here, then create our library to access the singleton and from the level scrip actor instantiate the sequence!
	//also create the multicast to "signal" when a target has been destroyed, we need to know when a wave has finished

	//empty
	//customLocations.Empty();

	

	//Sequences.Add(properties);
}

void UUnrealShooterDataSingleton::InitSequenceA()
{
	/*
	TArray<FTargetWave> Sequence;
	for (int i = 0; i < 11; i++)
	{
		TArray<FVector> customLocations;
		TArray<FRotatableTargetProperties> WaveData;

		WaveData.Add(FRotatableTargetProperties(FVector(1.0f, 1.0f, 0.0f), 2.0f, ETargetType::MaleTarget));

		//add this wave to sequence
		Sequence.Add(FTargetWave(WaveData));
	}
	*/
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
		const TArray<TSharedPtr<FJsonValue>> SequencesJSON = JsonObject->GetArrayField(TEXT("sequences"));
		const TArray<TSharedPtr<FJsonValue>> WavesJSON = JsonObject->GetArrayField(TEXT("waves"));
		const TArray<TSharedPtr<FJsonValue>> TargetsJSON = JsonObject->GetArrayField(TEXT("targets"));
		const TArray<TSharedPtr<FJsonValue>> LocationsJSON = JsonObject->GetArrayField("locations");

		UUnrealShooterDataSingleton::ParseLocations(LocationsJSON);
		UUnrealShooterDataSingleton::ParseTargets(TargetsJSON);

		

		//iterate over the array to get every sequence node
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *sequencesNode);
	}
}

void UUnrealShooterDataSingleton::ParseSequences(const TArray<TSharedPtr<FJsonValue>> &SequencesJSON)
{

}

void UUnrealShooterDataSingleton::ParseWaves(const TArray<TSharedPtr<FJsonValue>> &WavesJSON)
{

}

void UUnrealShooterDataSingleton::ParseTargets(const TArray<TSharedPtr<FJsonValue>> &TargetsJSON)
{
	for (int32 i = 0; i != TargetsJSON.Num(); ++i)
	{
		int32 targetID = TargetsJSON[i]->AsObject()->GetIntegerField(TEXT("targetID"));
		int32 initialLocationID = TargetsJSON[i]->AsObject()->GetIntegerField(TEXT("initialLocation"));
		int32 timeToLive = TargetsJSON[i]->AsObject()->GetIntegerField(TEXT("ttl"));
		FString targetType = TargetsJSON[i]->AsObject()->GetStringField(TEXT("targetType"));
		int32 speed = TargetsJSON[i]->AsObject()->GetIntegerField(TEXT("speed"));
		bool isExplosive = TargetsJSON[i]->AsObject()->GetBoolField(TEXT("isExplosive"));
		
		FVector initialLocation = GetTargetLocationByID(initialLocationID).Location;
		TArray<FVector> movementLocations;

		TArray<TSharedPtr<FJsonValue>> movementLocationsSubJSON = TargetsJSON[i]->AsObject()->GetArrayField(TEXT("movementLocations"));
		for (int32 j = 0; j != movementLocationsSubJSON.Num(); ++j)
		{
			int32 locationID = movementLocationsSubJSON[i]->AsNumber();// ->AsObject()->GetIntegerField(TEXT("speed"));
			movementLocations.Add(GetTargetLocationByID(locationID).Location);
		}
		Targets.Add(FRotatableTargetProperties(targetID, initialLocation, timeToLive, UUnrealShooterDataSingleton::GetEnumByString(targetType), movementLocations, speed, isExplosive));

		//UE_LOG(LogTemp, Warning, TEXT("Lo lograste no joda! -- ID: %d, Location: x(%f), y(%f)"), targetID, initialLocation.X, initialLocation.Y);
	}
}

void UUnrealShooterDataSingleton::ParseLocations(const TArray<TSharedPtr<FJsonValue>> &LocationsJSON)
{
	for (int32 i = 0; i != LocationsJSON.Num(); ++i)
	{
		int32 ID = LocationsJSON[i]->AsObject()->GetIntegerField(TEXT("locationID"));
		float x = LocationsJSON[i]->AsObject()->GetIntegerField(TEXT("x"));
		float y = LocationsJSON[i]->AsObject()->GetIntegerField(TEXT("y"));
		float z = LocationsJSON[i]->AsObject()->GetIntegerField(TEXT("z"));
		FVector targetLocation = { x, y, z };

		Locations.Add(FTargetLocation(ID, targetLocation));
		//UE_LOG(LogTemp, Warning, TEXT("Lo lograste no joda! -- ID: %d, Location: x(%f), y(%f)"), ID, x, y);
	}
}

ETargetType UUnrealShooterDataSingleton::GetEnumByString(FString const & inString)
{
	if (inString == "SpecialTarget") return ETargetType::SpecialTarget;
	if (inString == "InnocentTarget") return ETargetType::InnocentTarget;
	if (inString == "FemaleTarget") return ETargetType::FemaleTarget;
	if (inString == "MaleTarget") return ETargetType::MaleTarget;
	return ETargetType::MaleTarget;
}

FTargetLocation UUnrealShooterDataSingleton::GetTargetLocationByID(int32 locationID)
{
	for (int32 i = 0; i != Locations.Num(); ++i)
	{
		int32 myLocationID = Locations[i].LocationID;
		if (myLocationID == locationID)
		{
			return Locations[i];
		}
	}
	return FTargetLocation();
}