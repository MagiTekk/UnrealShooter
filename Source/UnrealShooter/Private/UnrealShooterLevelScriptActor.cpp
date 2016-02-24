// Fill out your copyright notice in the Description page of Project Settings.


//#include "BasicSpawnPoint.h"
//#include "RotatableTarget.h"
//#include "Engine/DestructibleMesh.h"

#include "UnrealShooter.h"
#include "TargetSequence.h"
#include "UnrealShooterDataSingleton.h"
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

	//UTargetSequence* SequenceA = new UTargetSequence();
	//NewObject()
	//UTargetSequence SequenceA = ConstructObject(UTargetSequence::StaticClass());
	

	/*UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	FTargetSequenceStruct sequenceProps = DataInstance->GetSequenceBySequenceName(TEXT("SequenceA"));
	SequenceA->ApplyProperties(sequenceProps.sequenceName, sequenceProps.WaveIDs);
	SequenceA->Play();*/
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

void AUnrealShooterLevelScriptActor::PlaySequence(int32 sequenceID)
{
	
}