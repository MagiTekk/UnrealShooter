// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "BasicSpawnPoint.h"
#include "RotatableTarget.h"
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
	FRotatableTargetStruct properties;
	
	InitialLocation = AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(1.0f, 0.0f, 0.0f));
	properties.TimeToLive = 10.0f;
	properties.TargetType = ETargetType::FalseTarget;
	
	ARotatableTarget* T1 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	T1->SetTargetProperties(properties);
	T1->SetActorLocation(InitialLocation);

	InitialLocation = AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(0.0f, 1.0f, 0.0f));
	properties.TimeToLive = 4.0f;
	properties.TargetType = ETargetType::MidTarget;

	ARotatableTarget* T2 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	T2->SetTargetProperties(properties);
	T2->SetActorLocation(InitialLocation);

	InitialLocation = AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(2.0f, 1.0f, 0.0f));
	properties.TimeToLive = 2.0f;
	properties.TargetType = ETargetType::LowTarget;

	ARotatableTarget* T3 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	T3->SetTargetProperties(properties);
	T3->SetActorLocation(InitialLocation);

	InitialLocation = AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(1.0f, 2.0f, 0.0f));
	properties.TimeToLive = 5.0f;
	properties.TargetType = ETargetType::FalseTarget;

	ARotatableTarget* T4 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	T4->SetTargetProperties(properties);
	T4->SetActorLocation(InitialLocation);
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