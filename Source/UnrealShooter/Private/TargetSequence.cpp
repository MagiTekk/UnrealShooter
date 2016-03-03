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

	TargetsAvailable = 0;

	//event handler
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnTargetDestroyed.AddDynamic(this, &UTargetSequence::OnTargetDestroyedHandler);
}

void UTargetSequence::OnTargetDestroyedHandler()
{
	//check if all the targets on this wave were destroyed
	TargetsAvailable--;
	//UE_LOG(LogTemp, Warning, TEXT("We still have: -- %d -- targets on the level"), TargetsAvailable);
	if (TargetsAvailable == 0)
	{
		//spawn the next wave
		PlayNextWave();
	}
}

void UTargetSequence::PlayNextWave()
{
	if (World)
	{
		_currentWave = GetNextWave();
		if (_currentWave.WaveID == -1.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayNextWave() - RELOAD TIME"));
			GEngine->AddOnScreenDebugMessage(-1, 3.0, FColor::Magenta, FString::FString("RELOAD TIME"));

			//this is because the wave is simply a "reload time" wave
			FTimerHandle THandle;
			World->GetTimerManager().SetTimer(THandle, this, &UTargetSequence::PlayNextWave, 3.0f, false);
		}
		else
		{
			ARotatableTarget* spawnedTarget;
			for (auto& props : _currentWave.Targets)
			{
				spawnedTarget = World->SpawnActor<ARotatableTarget>(TargetBP);
				spawnedTarget->ApplyProperties(props, _currentWave.TimeToLive);
				TargetsAvailable++;
			}
		}
	}
}

FTargetWave UTargetSequence::GetNextWave()
{
	//first wave not set
	if (_currentWave.WaveID != -1.0f && _currentWave.Targets.Num() == 0)
	{
		return Waves[0];
	}

	//for the rest of the waves
	for (int32 i = 0; i != Waves.Num(); ++i)
	{
		if (Waves[i].WaveID == _currentWave.WaveID)
		{
			if (i + 1 < Waves.Num())
			{
				return Waves[i + 1];
			}
		}
	}
	return FTargetWave();
}


