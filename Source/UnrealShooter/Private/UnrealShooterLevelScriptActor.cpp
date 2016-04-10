// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "UnrealHUD.h"
#include "UnrealShooterLevelScriptActor.h"


// Constructor
AUnrealShooterLevelScriptActor::AUnrealShooterLevelScriptActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UClass> camShake(TEXT("Blueprint'/Game/UnrealShooter/Blueprint/Effects/SpecialTargetCameraShake.SpecialTargetCameraShake_C'"));
	CameraShakeBP = camShake.Object;

	WorldReference = GetWorld();
}

// Called when the game starts or when spawned
void AUnrealShooterLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	ResetTargetsHit();
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

void AUnrealShooterLevelScriptActor::ResetTargetsHit()
{
	TargetsHit = 0;
}

void AUnrealShooterLevelScriptActor::RewardTargetPoints(int32 points, FVector Location)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, FString::FString(FString::FromInt(points)));
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		AUnrealHUD* HUD = Cast<AUnrealHUD>(PC->GetHUD());
		if (HUD)
		{
			HUD->RewardTargetPoints(points, Location);
		}
	}
}

void AUnrealShooterLevelScriptActor::CameraShake()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		PC->ClientPlayCameraShake(CameraShakeBP, 10.0f);
	}
}

void AUnrealShooterLevelScriptActor::OnTargetHit()
{
	TargetsHit++;
	if (TargetsHit % 5 == 0)
	{
		CurrentSequence->SpawnSpecialTarget();
	}
}

void AUnrealShooterLevelScriptActor::PlaySequence(ESequenceEnum sequenceType)
{
	CurrentSequence = NewObject<UTargetSequence>(UTargetSequence::StaticClass());
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	FTargetSequenceStruct sequenceProps = DataInstance->GetSequenceBySequenceEnum(sequenceType);
	CurrentSequence->ApplyProperties(sequenceProps.sequenceName, sequenceProps.Waves, GetWorld());
	CurrentSequence->PlayNextWave();
}