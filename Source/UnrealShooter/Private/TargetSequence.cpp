// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "BasicSpawnPoint.h"
#include "RotatableTarget.h"
#include "Engine/DestructibleMesh.h"
#include "UnrealShooterDataSingleton.h"
#include "TargetSequence.h"

UTargetSequence::UTargetSequence()
{
	ConstructorHelpers::FObjectFinder<UClass> Target(TEXT("Blueprint'/Game/UnrealShooter/Blueprint/Target/TargetDummyBP.TargetDummyBP_C'"));
	TargetBP = Target.Object;

	/*
	ConstructorHelpers::FObjectFinder<UClass> TargetCylinder(TEXT("Blueprint'/Game/UnrealShooter/Blueprint/Target/TargetCylinder_BP.TargetCylinder_BP_C'"));
	TargetCylinderBP = TargetCylinder.Object;
	*/
}

void UTargetSequence::ApplyProperties(FString sequenceName, TArray<FTargetWave> Waves, UWorld* World)
{
	this->sequenceName = sequenceName;
	this->Waves = Waves;
	this->World = World;

	//event handler
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnTargetDestroyed.AddDynamic(this, &UTargetSequence::OnTargetDestroyedHandler);
}

void UTargetSequence::OnTargetDestroyedHandler(int32 TargetID)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Yellow, FString::FString("ERICK_YOU_ARE_FUCKING_AWESOME!!!!"));

	UE_LOG(LogTemp, Warning, TEXT("TargetID: %d --- The current amount of targets in this wave are: %d"), TargetID, _currentWave.Targets.Num());
	RemoveTargetFromCurrentWave(TargetID);
	UE_LOG(LogTemp, Warning, TEXT("After removal: %d"), _currentWave.Targets.Num());*/
}

void UTargetSequence::Play()
{
	if (World)
	{
		GetNextWave();
		ARotatableTarget* spawnedTarget;
		for (int32 i = 0; i != _currentWave.Targets.Num(); ++i)
		{
			FRotatableTargetProperties myTargetProps = _currentWave.Targets[i];
			spawnedTarget = World->SpawnActor<ARotatableTarget>(TargetBP);
			spawnedTarget->ApplyProperties(myTargetProps);
		}
	}
}

void UTargetSequence::GetNextWave()
{
	if (Waves.Num() > 0)
	{
		_currentWave = Waves[0];
		Waves.RemoveAt(0);
	}
}

/*
	Working on this method, this one is causing errors since many
	targets access the function at the same time and modify the array,
	find a better way to handle the removal from the array
*/
void UTargetSequence::RemoveTargetFromCurrentWave(int32 TargetID)
{
	for (int32 i = 0; i != _currentWave.Targets.Num(); ++i)
	{
		if (i >= 0 && i < _currentWave.Targets.Num())
		{
			const FRotatableTargetProperties myTargetProps = _currentWave.Targets[i];
			if (TargetID == myTargetProps.TargetID)
			{
				_currentWave.Targets.RemoveAt(i);
			}
		}
	}
}


