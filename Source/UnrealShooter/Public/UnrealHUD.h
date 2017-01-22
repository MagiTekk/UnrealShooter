// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "UnrealHUD.generated.h"

USTRUCT()
struct FHUDPointsProperties
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 Points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float FakeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bMarkedForDestruction = false;

	FHUDPointsProperties()
	{
	}

	FHUDPointsProperties(int32 Points, FVector Location, float FakeTimer)
	{
		this->Points = Points;
		this->Location = Location;
		this->FakeTimer = FakeTimer;
	}
};

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API AUnrealHUD : public AHUD
{
	GENERATED_BODY()
	
private:

	TArray<FHUDPointsProperties> PointsDisplayedOnScreen;
	void DrawRewardedPoints();
	void RemoveMarkedElement();

public:

	AUnrealHUD();

	virtual void DrawHUD() override;

	const int32 HIT_POINTS_OFFSET = 20;

	UPROPERTY()
		UFont* HUDFont;

	UFUNCTION()
		void RewardTargetPoints(int32 points, FVector Location);
};
