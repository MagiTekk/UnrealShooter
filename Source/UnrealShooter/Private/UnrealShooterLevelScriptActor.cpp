// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "BasicSpawnPoint.h"
#include "RotatableTarget.h"
#include "UnrealShooterStructData.h"
#include "Engine/DestructibleMesh.h"
#include "UnrealShooterLevelScriptActor.h"


// Constructor
AUnrealShooterLevelScriptActor::AUnrealShooterLevelScriptActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UClass> Target(TEXT("Blueprint'/Game/UnrealShooter/Blueprint/Target/TargetDummyBP.TargetDummyBP_C'"));
	TargetBP = Target.Object;

	ConstructorHelpers::FObjectFinder<UClass> TargetCylinder(TEXT("Blueprint'/Game/UnrealShooter/Blueprint/Target/TargetCylinder_BP.TargetCylinder_BP_C'"));
	TargetCylinderBP = TargetCylinder.Object;
}

// Called when the game starts or when spawned
void AUnrealShooterLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	AUnrealShooterLevelScriptActor::InitiateSequence_0();
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

void AUnrealShooterLevelScriptActor::InitiateSequence_0()
{
	FVector InitialLocation;
	FRotatableTargetProperties properties;
	TArray<FVector> customLocations;
	
	InitialLocation = AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(1.0f, 0.0f, 0.0f));
	FVector Arr[] = {	AUnrealShooterLevelScriptActor::GetSpawnPoint({ 0.0f, 0.0f, 0.0f }), AUnrealShooterLevelScriptActor::GetSpawnPoint({ 0.0f, 1.0f, 0.0f }),
						AUnrealShooterLevelScriptActor::GetSpawnPoint({ 1.0f, 1.0f, 0.0f }) };

	customLocations.Append(Arr, ARRAY_COUNT(Arr));
	properties = FRotatableTargetProperties(InitialLocation, 1.0f, ETargetType::InnocentTarget, customLocations);
	Sequence_0.Add(properties);
	
	ARotatableTarget* T1 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	T1->ApplyProperties(properties);

	InitialLocation = AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(0.0f, 2.0f, 0.0f));
	properties = FRotatableTargetProperties(InitialLocation, 4.0f, ETargetType::FemaleTarget);
	Sequence_0.Add(properties);

	ARotatableTarget* T2 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	T2->ApplyProperties(properties);

	InitialLocation = AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(2.0f, 1.0f, 0.0f));
	//properties = FRotatableTargetStruct(InitialLocation, 2.0f, ETargetType::LowTarget);
	Sequence_0.Add(FRotatableTargetProperties(InitialLocation, 2.0f, ETargetType::MaleTarget));

	//ARotatableTarget* T3 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	//T3->ApplyProperties(properties);

	InitialLocation = AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(1.0f, 2.0f, 0.0f));
	//properties = FRotatableTargetStruct(InitialLocation, 5.0f, ETargetType::FalseTarget);
	Sequence_0.Add(FRotatableTargetProperties(InitialLocation, 5.0f, ETargetType::InnocentTarget));

	//ARotatableTarget* T4 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	//T4->ApplyProperties(properties);

	/****	NOTE sequences hold waves of targets, but sequences need to keep track of each wave to know if it was destroyed or not to spawn the next one,
			to listen to this we need to implement a game singleton object, in there we will set up an event habdler component that will broadcast signals around ****/
	
}

void AUnrealShooterLevelScriptActor::PlaySequence(int32 sequenceID)
{
	
}

FVector AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector SpawnPosition)
{
	for (TActorIterator<ABasicSpawnPoint> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->SpawnPosition == SpawnPosition)
		{
			return ActorItr->GetActorLocation();
		}
	}
	return FVector();
}