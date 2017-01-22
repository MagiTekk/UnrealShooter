// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "UnrealHUD.h"


AUnrealHUD::AUnrealHUD()
{
	ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("Font'/Engine/EngineFonts/RobotoDistanceField.RobotoDistanceField'"));
	HUDFont = Font.Object;
}

void AUnrealHUD::DrawHUD()
{
	Super::DrawHUD();

	if (PointsDisplayedOnScreen.Num() > 0)
	{
		DrawRewardedPoints();
	}
}

void AUnrealHUD::RewardTargetPoints(int32 points, FVector Location)
{
	PointsDisplayedOnScreen.Emplace(FHUDPointsProperties(points, Location, 255.0f));
}

void AUnrealHUD::DrawRewardedPoints()
{
	int32 deletionCounter = 0;
	for (int32 Index = 0; Index != PointsDisplayedOnScreen.Num(); ++Index)
	{
		auto& props = PointsDisplayedOnScreen[Index];
		FVector projectedLocation = props.Location.Projection();
		FString textToDraw = (props.Points >= 0 ? TEXT("+") : TEXT("")) + FString::FromInt(props.Points);
		FVector2D projectedLocationToDraw(projectedLocation.X + (HIT_POINTS_OFFSET * Index), projectedLocation.Y - (HIT_POINTS_OFFSET * Index));
		FVector2D fontScaleToDraw(1.0f, 1.0f);
		int32 negativeColor = props.Points >= 0 ? 255 : 0;
		const FColor AlphaWhite(255, negativeColor, negativeColor, props.FakeTimer); //always white but red for bad target

		//Only proceed if the objective is in front of the player?
		if (projectedLocation.Z > 0)
		{
			props.FakeTimer -= 1.5f;
			if (props.FakeTimer < 0 && !props.bMarkedForDestruction)
			{
				deletionCounter++;
				props.bMarkedForDestruction = true;
			}
			else if (props.FakeTimer >= 0)
			{
				DrawText(textToDraw, AlphaWhite, projectedLocationToDraw.X, projectedLocationToDraw.Y, HUDFont);
			}
		}
	}

	//remove transparent points, one at a time to avoid index out of bounds
	for (int32 i = 0; i < deletionCounter; ++i)
	{
		RemoveMarkedElement();
	}
}

void AUnrealHUD::RemoveMarkedElement()
{
	for (int32 Index = 0; Index != PointsDisplayedOnScreen.Num(); ++Index)
	{
		auto& element = PointsDisplayedOnScreen[Index];
		if (element.bMarkedForDestruction)
		{
			PointsDisplayedOnScreen.RemoveAt(Index);
			break;
		}
	}
}
