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

#pragma region Widgets

public:

	/*
	note that "class" in front UWidget, this is called forward declaration and it makes the compiler know this is a class
	even withot having the included header, but you need to make sure to include the header on the .cpp or it will break.
	*This prevents circular dependecies* :)
	*/

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UHUDUserWidget> wHUD;

	// Variable to hold the widget After Creating it.
	UPROPERTY()
		UHUDUserWidget* HUDReference;

#pragma endregion
	
};
