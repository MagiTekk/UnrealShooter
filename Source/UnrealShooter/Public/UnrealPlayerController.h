// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "UnrealPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API AUnrealPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AUnrealPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

#pragma region Widgets

public:

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UMainMenuWidget> wStartScreen;

	// Variable to hold the widget After Creating it.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		UMainMenuWidget* StartScreenReference;

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UHUDUserWidget> wHUD;

	// Variable to hold the widget After Creating it.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		UHUDUserWidget* HUDReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wPauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wSequenceSelector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		UUserWidget* SequenceSelectorReference;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		UUserWidget* PauseMenuReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wFinisher;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		UUserWidget* FinisherReference;

	/* Make sure this is set to true every time a selectable widget is shown */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		bool bIsWidgetShown;

	/* Is this blueprint going to be possesed by an actor? then that pawn should take care of input bindings */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possesion")
		bool bIsControllerPossesed;

	UFUNCTION()
		void AddHUD();

	UFUNCTION()
		void AddStartScreen();

	UFUNCTION()
		void ShowPauseMenu();

	UFUNCTION()
		void ShowSequenceSelector();

	UFUNCTION()
		void ShowFinisher();

	UFUNCTION()
		void StartReloadTime();

	UFUNCTION()
		void OnReloadTimeFinished();

#pragma endregion
#pragma region ActionMappings

public:
	UFUNCTION()
		void UINavigationUp();

	UFUNCTION()
		void UINavigationDown();

	UFUNCTION()
		void UINavigationLeft();

	UFUNCTION()
		void UINavigationRight();

	UFUNCTION()
		void UINavigation(FVector2D direction);

	UFUNCTION()
		void UISelectElement();
	
#pragma endregion
};
