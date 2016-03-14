// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "HUDUserWidget.h"
#include "UnrealPlayerController.h"


AUnrealPlayerController::AUnrealPlayerController()
{

}

void AUnrealPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (wHUD) // Check if the Asset is assigned in the blueprint.
	{
		// Create the widget and store it.
		HUDReference = CreateWidget<UHUDUserWidget>(this, wHUD);

		// now you can use the widget directly since you have a referance for it.
		// Extra check to  make sure the pointer holds the widget.
		if (HUDReference)
		{
			//let add it to the view port
			HUDReference->AddToViewport();
			HUDReference->InitWidget(GetWorld());
		}

		//Show the Cursor.
		bShowMouseCursor = true;
	}
}