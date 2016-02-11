// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "BasicSpawnPoint.h"
#include "RotatableTarget.h"
#include "UnrealShooterLevelScriptActor.h"


// Constructor
AUnrealShooterLevelScriptActor::AUnrealShooterLevelScriptActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UClass> Target(TEXT("Blueprint'/Game/UnrealShooter/Blueprint/Target/TargetDummyBP.TargetDummyBP_C'"));
	if (Target.Object)
	{
		TargetBP = Target.Object;
	}
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
	// Test spawn 4 targets
	AActor* T1 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	T1->SetActorLocation(AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(2.0f, 0.0f, 0.0f)));

	AActor* T2 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	T2->SetActorLocation(AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(1.0f, 1.0f, 0.0f)));

	AActor* T3 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	T3->SetActorLocation(AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(0.0f, 2.0f, 0.0f)));

	AActor* T4 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	T4->SetActorLocation(AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(2.0f, 3.0f, 0.0f)));

	AActor* T5 = GetWorld()->SpawnActor<ARotatableTarget>(TargetBP);
	T5->SetActorLocation(AUnrealShooterLevelScriptActor::GetSpawnPoint(FVector(0.0f, 4.0f, 0.0f)));
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