// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "MainMenuWidget.h"
#include "MainCharacter.h"
#include "Runtime/Engine/Classes/Matinee/MatineeActor.h"
#include "GameFramework/PlayerController.h"
#include "StartScreenScriptActor.h"

AStartScreenScriptActor::AStartScreenScriptActor()
{
	//just forget about referencing a widget from code and add it to viewport, it's madness!! (I don't even think it's possible)
	//ConstructorHelpers::FObjectFinder<UClass> wStartScreenRef(TEXT("WidgetBlueprint'/Game/UnrealShooter/UMG/BP_MainMenu.BP_MainMenu_C'"));
	//wStartScreen = wStartScreenRef.Object;
}

// Called when the game starts or when spawned
void AStartScreenScriptActor::BeginPlay()
{
	Super::BeginPlay();
	/*
	if (wStartScreen) // Check if the Asset is assigned in the blueprint.
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			//let add it to the view port
			StartScreenReference = CreateWidget<UMainMenuWidget>(PC, wStartScreen);
			StartScreenReference->AddToViewport();

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(StartScreenReference->GetCachedWidget());
			PC->SetInputMode(InputMode);

			EnableInput(PC);

			StartScreenReference->SetKeyboardFocus();

			//Show the Cursor.
			PC->bShowMouseCursor = true;
		}


		TArray<AMatineeActor*> OutMatineeActors;
		GetWorld()->GetMatineeActors(OutMatineeActors);
		AMatineeActor* MyMatinee = OutMatineeActors[0];
		if (MyMatinee)
		{
			MyMatinee->Play();
			MyMatinee->SetLoopingState(true);
		}
	}
	*/
}
