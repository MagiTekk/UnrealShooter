// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LevelScriptActor.h"
#include "StartScreenScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API AStartScreenScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
	
public:

	AStartScreenScriptActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UMainMenuWidget> wStartScreen;

	// Variable to hold the widget After Creating it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UMainMenuWidget* StartScreenReference;
	
};
