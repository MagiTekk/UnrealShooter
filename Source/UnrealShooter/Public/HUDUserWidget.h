// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "HUDUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API UHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void InitWidget(UWorld* World);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
		class AMainCharacter* MyCharacter;
	
};
