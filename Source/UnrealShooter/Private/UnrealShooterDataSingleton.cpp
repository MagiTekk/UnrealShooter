// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "UnrealShooterDataSingleton.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"


UUnrealShooterDataSingleton::UUnrealShooterDataSingleton()
{
	UUnrealShooterDataSingleton::ParseJSON();
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
		UUnrealShooterDataSingleton::ParseWaves(WavesJSON);
		UUnrealShooterDataSingleton::ParseSequences(SequencesJSON);
	}
}

void UUnrealShooterDataSingleton::ParseSequences(const TArray<TSharedPtr<FJsonValue>> &SequencesJSON)
{
	for (int32 i = 0; i != SequencesJSON.Num(); ++i)
	{
		FString sequenceName = SequencesJSON[i]->AsObject()->GetStringField(TEXT("sequenceName"));

		TArray<int32> waveIDs;
		TArray<TSharedPtr<FJsonValue>> waveIDsSubJSON = SequencesJSON[i]->AsObject()->GetArrayField(TEXT("waveIDs"));
		for (int32 j = 0; j != waveIDsSubJSON.Num(); ++j)
		{
			int32 waveID = waveIDsSubJSON[j]->AsNumber();
			waveIDs.Add(waveID);
		}

		//UTargetSequence* newSequence = Cast<UTargetSequence>(UTargetSequence::StaticClass()->GetDefaultObject());
		//newSequence->ApplyProperties(sequenceName, waveIDs);
		
		Sequences.Add(FTargetSequenceStruct(sequenceName, waveIDs));
		//UE_LOG(LogTemp, Warning, TEXT("Lo lograste no joda! -- sequenceName: %s, waveID[0]: %d"), *newSequence->sequenceName, newSequence->WaveIDs[0]);
		//UE_LOG(LogTemp, Warning, TEXT("Lo lograste no joda! -- sequenceName: %s, waveID[0]: %d"), *sequenceName, waveIDs[1]);
	}
}

void UUnrealShooterDataSingleton::ParseWaves(const TArray<TSharedPtr<FJsonValue>> &WavesJSON)
{
	for (int32 i = 0; i != WavesJSON.Num(); ++i)
	{
		int32 waveID = WavesJSON[i]->AsObject()->GetIntegerField(TEXT("waveID"));
		
		TArray<int32> targetIDs;
		TArray<TSharedPtr<FJsonValue>> targetIDsSubJSON = WavesJSON[i]->AsObject()->GetArrayField(TEXT("targetIDs"));
		for (int32 j = 0; j != targetIDsSubJSON.Num(); ++j)
		{
			int32 targetID = targetIDsSubJSON[j]->AsNumber();
			targetIDs.Add(targetID);
			//UE_LOG(LogTemp, Warning, TEXT("Lo lograste no joda! -- ID: %d, targetID(%d)"), waveID, targetID);
		}
		Waves.Add(FTargetWave(waveID, targetIDs));
	}
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