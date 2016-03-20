// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "HUDUserWidget.h"
#include "MainMenuWidget.h"
#include "UnrealPlayerController.h"


AUnrealPlayerController::AUnrealPlayerController()
{

}

void AUnrealPlayerController::BeginPlay()
{
	Super::BeginPlay();
	AddHUD();
}

void AUnrealPlayerController::AddHUD()
{
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
			HUDReference->InitWidget();
		}

		//Show the Cursor.
		//bShowMouseCursor = true;
	}
}

void AUnrealPlayerController::ShowPauseMenu()
{
	if (wPauseMenu) // Check if the Asset is assigned in the blueprint.
	{
		// Create the widget and store it.
		PauseMenuReference = CreateWidget<UUserWidget>(this, wPauseMenu);

		// now you can use the widget directly since you have a referance for it.
		// Extra check to  make sure the pointer holds the widget.
		if (PauseMenuReference)
		{
			FInputModeGameAndUI Mode;
			Mode.SetWidgetToFocus(PauseMenuReference->GetCachedWidget());
			SetInputMode(Mode);

			//let add it to the view port
			PauseMenuReference->AddToViewport();
			SetPause(true);
		}

		//Show the Cursor.
		bShowMouseCursor = true;
	}
}