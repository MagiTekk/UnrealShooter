// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "UnrealShooterLevelScriptActor.h"


// Constructor
AUnrealShooterLevelScriptActor::AUnrealShooterLevelScriptActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUnrealShooterLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	
	/*CurrentSequence = NewObject<UTargetSequence>(UTargetSequence::StaticClass());
	
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	FTargetSequenceStruct sequenceProps = DataInstance->GetSequenceBySequenceName(TEXT("SequenceB"));
	CurrentSequence->ApplyProperties(sequenceProps.sequenceName, sequenceProps.Waves, GetWorld());
	CurrentSequence->PlayNextWave();*/
	
	/*
	UTargetSequence* Sequence_B = NewObject<UTargetSequence>(UTargetSequence::StaticClass());

	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	FTargetSequenceStruct sequenceProps = DataInstance->GetSequenceBySequenceName(TEXT("SequenceB"));
	Sequence_B->ApplyProperties(sequenceProps.sequenceName, sequenceProps.Waves, GetWorld());
	Sequence_B->PlayNextWave();
	*/
}

// Called every frame
void AUnrealShooterLevelScriptActor::Tick(float DeltaTime)
{

}

// Called every frame
void AUnrealShooterLevelScriptActor::BeginDestroy()
{
	Super::BeginDestroy();
}

void AUnrealShooterLevelScriptActor::PlaySequence(ESequenceEnum sequenceType)
{
	CurrentSequence = NewObject<UTargetSequence>(UTargetSequence::StaticClass());
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	FTargetSequenceStruct sequenceProps = DataInstance->GetSequenceBySequenceEnum(sequenceType);
	CurrentSequence->ApplyProperties(sequenceProps.sequenceName, sequenceProps.Waves, GetWorld());
	CurrentSequence->PlayNextWave();
}