// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "TargetSequence.h"

UTargetSequence::UTargetSequence()
{

}

void UTargetSequence::ApplyProperties(FString sequenceName, TArray<int32> WaveIDs)
{
	this->sequenceName = sequenceName;
	this->WaveIDs = WaveIDs;
}


