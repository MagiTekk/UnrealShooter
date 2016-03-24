// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "HUDUserWidget.h"
#include "MainMenuWidget.h"
#include "UnrealShooterDataSingleton.h"
#include "Runtime/Engine/Classes/Matinee/MatineeActor.h"
#include "UnrealPlayerController.h"


AUnrealPlayerController::AUnrealPlayerController()
{

}

void AUnrealPlayerController::BeginPlay()
{
	Super::BeginPlay();
	AddHUD();
	AddStartScreen();
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
	}
}

void AUnrealPlayerController::AddStartScreen()
{
	if (wStartScreen) // Check if the Asset is assigned in the blueprint.
	{
		//let add it to the view port
		StartScreenReference = CreateWidget<UMainMenuWidget>(this, wStartScreen);
		StartScreenReference->AddToViewport();
		bIsWidgetShown = true;

		FInputModeGameAndUI InputMode;
		//InputMode.SetWidgetToFocus(StartScreenReference->GetCachedWidget());
		SetInputMode(InputMode);

		//StartScreenReference->SetKeyboardFocus();

		//Show the Cursor.
		bShowMouseCursor = true;

		TArray<AMatineeActor*> OutMatineeActors;
		GetWorld()->GetMatineeActors(OutMatineeActors);
		AMatineeActor* MyMatinee = OutMatineeActors[0];
		if (MyMatinee)
		{
			MyMatinee->Play();
			MyMatinee->SetLoopingState(true);
		}
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
			//let add it to the view port
			PauseMenuReference->AddToViewport();
			SetPause(true);
			bIsWidgetShown = true;

			FInputModeGameAndUI Mode;
			SetInputMode(Mode);
		}

		//Show the Cursor.
		bShowMouseCursor = true;
	}
}

void AUnrealPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!bIsControllerPossesed)
	{
		InputComponent->BindAction("UINavigationUp", IE_Pressed, this, &AUnrealPlayerController::UINavigationUp);
		InputComponent->BindAction("UINavigationDown", IE_Pressed, this, &AUnrealPlayerController::UINavigationDown);
		InputComponent->BindAction("UINavigationLeft", IE_Pressed, this, &AUnrealPlayerController::UINavigationLeft);
		InputComponent->BindAction("UINavigationRight", IE_Pressed, this, &AUnrealPlayerController::UINavigationRight);
		InputComponent->BindAction("UISelectElement", IE_Pressed, this, &AUnrealPlayerController::UISelectElement);
	}
}

void AUnrealPlayerController::UINavigationUp()
{
	UINavigation(FVector2D(0.0f, -1.0f));
}

void AUnrealPlayerController::UINavigationDown()
{
	UINavigation(FVector2D(0.0f, 1.0f));
}

void AUnrealPlayerController::UINavigationLeft()
{
	UINavigation(FVector2D(-1.0f, 0.0f));
}

void AUnrealPlayerController::UINavigationRight()
{
	UINavigation(FVector2D(1.0f, 0.0f));
}

void AUnrealPlayerController::UINavigation(FVector2D direction)
{
	if (bIsWidgetShown)
	{
		UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
		DataInstance->OnUINavigation.Broadcast(direction);
		//UE_LOG(LogTemp, Warning, TEXT("UINavigationRight, valueX: %f __ valueY: %f"), direction.X, direction.Y);
	}
}

void AUnrealPlayerController::UISelectElement()
{
	if (bIsWidgetShown)
	{
		UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
		DataInstance->OnUISelection.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("UISelectElement"));
	}
}
