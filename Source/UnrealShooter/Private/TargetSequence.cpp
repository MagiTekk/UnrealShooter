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

void UTargetSequence::OnTargetDestroyedHandler()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Yellow, FString::FString("ERICK_YOU_ARE_FUCKING_AWESOME!!!!"));
}

void UTargetSequence::Play()
{
	//FVector InitialLocation = AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(0.0f, 2.0f, 0.0f));
	//FRotatableTargetProperties properties = FRotatableTargetProperties(InitialLocation, 4.0f, ETargetType::FemaleTarget);
	//FRotatableTargetProperties properties = Cast<FRotatableTargetProperties>(Waves[0].Targets[0]);

	//ARotatableTarget* T2 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	//T2->ApplyProperties(Waves[0].Targets[0]);

	if (World)
	{
		ARotatableTarget* T1 = World->SpawnActor<ARotatableTarget>(TargetBP);
		T1->ApplyProperties(Waves[0].Targets[0]);
	}
}


