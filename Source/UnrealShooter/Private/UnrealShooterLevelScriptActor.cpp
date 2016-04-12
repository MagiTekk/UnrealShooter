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
	ChainHits = 0;
}

void AUnrealShooterLevelScriptActor::DisplayRewardedPoints(int32 points, FVector Location)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		AUnrealHUD* HUD = Cast<AUnrealHUD>(PC->GetHUD());
		if (HUD)
		{
			Points += points;
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

void AUnrealShooterLevelScriptActor::OnTargetHit(bool bIsHeadshot, bool bIsBonusTarget)
{
	if (!bIsBonusTarget)
	{
		ChainHits++;
		AmountNormalHits += !bIsHeadshot ? 1 : 0;
		AmountHeadshotHits += bIsHeadshot ? 1 : 0;
	}
	else
	{
		ChainHits = 0;
		AmountBonusHits++;
	}

	if (ChainHits != 0 && ChainHits % 5 == 0)
	{
		if (CurrentSequence)
		{
			CurrentSequence->SpawnSpecialTarget();
		}
	}
}

void AUnrealShooterLevelScriptActor::InitSequenceValues()
{
	Points = 0;
	ChainHits = 0;
	AmountNormalHits = 0;
	AmountHeadshotHits = 0;
}

void AUnrealShooterLevelScriptActor::PlaySequence(ESequenceEnum sequenceType)
{
	InitSequenceValues();
	CurrentSequenceName = GetSequenceEnumCodeName(sequenceType);

	CurrentSequence = NewObject<UTargetSequence>(UTargetSequence::StaticClass());
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	FTargetSequenceStruct sequenceProps = DataInstance->GetSequenceBySequenceEnum(sequenceType);
	CurrentSequence->ApplyProperties(sequenceProps.sequenceName, sequenceProps.Waves, GetWorld());
	CurrentSequence->PlayNextWave();
}

FName AUnrealShooterLevelScriptActor::GetSequenceEnumCodeName(ESequenceEnum sequenceType)
{
	switch (sequenceType)
	{
		case ESequenceEnum::SequenceA:
			return TEXT("A");
			break;
		case ESequenceEnum::SequenceB:
			return TEXT("B");
			break;
		case ESequenceEnum::SequenceC:
			return TEXT("C");
			break;
		case ESequenceEnum::SequenceD:
			return TEXT("D");
			break;
		case ESequenceEnum::SequenceE:
			return TEXT("E");
			break;
	}
	return TEXT("");
}