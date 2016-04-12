// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "BasicButton.h"
#include "BasicSpawnPoint.h"
#include "SpecialTarget.h"
#include "RotatableTarget.h"
#include "Engine/DestructibleMesh.h"
#include "UnrealShooterDataSingleton.h"
#include "UnrealPlayerController.h"
#include "TargetSequence.h"

UTargetSequence::UTargetSequence()
{
	ConstructorHelpers::FObjectFinder<UClass> Target(TEXT("Blueprint'/Game/UnrealShooter/Blueprint/Target/TargetDummyBP.TargetDummyBP_C'"));
	TargetBP = Target.Object;

	ConstructorHelpers::FObjectFinder<UClass> specialTargetReference(TEXT("Blueprint'/Game/UnrealShooter/Blueprint/Target/SpecialTargetBP.SpecialTargetBP_C'"));
	SpecialTargetBP = specialTargetReference.Object;
}

void UTargetSequence::ApplyProperties(FString sqName, TArray<FTargetWave> SequenceWaves, UWorld* theWorld)
{
	this->sequenceName = sqName;
	this->Waves = SequenceWaves;
	this->World = theWorld;

	TargetsAvailable = 0;

	//event handler
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnTargetDestroyed.AddDynamic(this, &UTargetSequence::OnTargetDestroyedHandler);
}

void UTargetSequence::SpawnSpecialTarget()
{
	if (World)
	{
		//Spawn special target
		ASpecialTarget* targetSpawned = World->SpawnActor<ASpecialTarget>(SpecialTargetBP);

		//default destination
		FVector Arr[] = { FVector(101.0f, 101.0f, 0.0f) };
		TArray<FVector> locations;
		locations.Append(Arr, ARRAY_COUNT(Arr));

		targetSpawned->ApplyProperties(FSpecialTargetProperties(FVector(100.0f, 100.0f, 0.0f), 0.0f, DEFAULT_SPECIAL_TARGET_POINTS, locations));
		TargetsAvailable++;
	}
}

void UTargetSequence::OnTargetDestroyedHandler(AActor* Target)
{
	//check if all the targets on this wave were destroyed
	TargetsAvailable--;
	UpdateTargetsThisWave(Target);
	
	if (TargetsAvailable == 0)
	{
		//spawn the next wave
		PlayNextWave();
	}
	else if (NormalTargetsThisWave <= 0 && InnocentTargetsThisWave > 0)
	{
		//find out if an innocent target is left, if that's true then destroy it
		for (auto& target : _currentWaveTargets)
		{
			if (target->TargetProperties.TargetType == ETargetType::InnocentTarget)
			{
				target->FakeTargetHit();
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("We still have: -- %d -- targets on the level"), TargetsAvailable);
}

void UTargetSequence::UpdateTargetsThisWave(AActor* Target)
{
	ARotatableTarget* rotTarget = Cast<ARotatableTarget>(Target);
	if (rotTarget)
	{
		if (rotTarget->TargetProperties.TargetType != ETargetType::InnocentTarget)
		{
			NormalTargetsThisWave--;
		}
		else
		{
			InnocentTargetsThisWave--;
		}
	}
}

void UTargetSequence::PlayNextWave()
{
	if (World)
	{
		NormalTargetsThisWave = 0;
		InnocentTargetsThisWave = 0;
		_currentWaveTargets.Empty();
		_currentWave = GetNextWave();

		//is this a "reload time" wave?
		if (_currentWave.WaveID == -1.0f)
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
			if (PC)
			{
				AUnrealPlayerController* const UPC = CastChecked<AUnrealPlayerController>(PC);
				if (UPC)
				{
					UPC->StartReloadTime();
				}
			}
			World->GetTimerManager().SetTimer(TimerHandle, this, &UTargetSequence::ReloadTimeFinished, 4.0f, false);
			//UE_LOG(LogTemp, Warning, TEXT("PlayNextWave() - RELOAD TIME"));
			//GEngine->AddOnScreenDebugMessage(-1, 4.0, FColor::Magenta, FString::FString("RELOAD TIME"));
		}
		else if (_currentWave.Targets.Num() > 0)
		{
			ARotatableTarget* spawnedTarget;
			for (auto& props : _currentWave.Targets)
			{
				spawnedTarget = World->SpawnActor<ARotatableTarget>(TargetBP);
				spawnedTarget->ApplyProperties(props, _currentWave.TimeToLive);
				TargetsAvailable++;
				_currentWaveTargets.Emplace(spawnedTarget);

				if (props.TargetType == ETargetType::InnocentTarget)
				{
					InnocentTargetsThisWave++;
				}
				else 
				{
					NormalTargetsThisWave++;
				}
			}
		}
		else
		{
			//Sequence finished!
			World->GetTimerManager().SetTimer(FinisherHandle, this, &UTargetSequence::OnSequenceFinished, 2.0f, false);
		}
	}
}

void UTargetSequence::ReloadTimeFinished()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (PC)
	{
		AUnrealPlayerController* const UPC = CastChecked<AUnrealPlayerController>(PC);
		if (UPC)
		{
			UPC->OnReloadTimeFinished();
		}
	}
	PlayNextWave();
}

void UTargetSequence::OnSequenceFinished()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (PC)
	{
		AUnrealPlayerController* const UPC = CastChecked<AUnrealPlayerController>(PC);
		if (UPC)
		{
			//Close the doors (if open) and show finisher
			UPC->ShowFinisher();
		}
	}
}

FTargetWave UTargetSequence::GetNextWave()
{
	//first wave not set
	if (_currentWave.WaveID != -1 && _currentWave.WaveID >= 0 && _currentWave.Targets.Num() == 0)
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

void UTargetSequence::ReactivatePlayWavesButton()
{
	for (TActorIterator<ABasicButton> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ABasicButton *Button = *ActorItr;
		//UE_LOG(LogTemp, Warning, TEXT("ReactivatePlayWavesButton - %s"), *ActorItr->GetName());
		if (Button->GetName() == "SequenceButton")
		{
			Button->ActivateButton();
			break;
		}
	}
}


