// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "UnrealShooterLevelScriptActor.h"


// Constructor
AUnrealShooterLevelScriptActor::AUnrealShooterLevelScriptActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	WorldReference = GetWorld();
}

// Called when the game starts or when spawned
void AUnrealShooterLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
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