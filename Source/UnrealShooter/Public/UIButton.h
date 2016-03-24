// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UIButton.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API UUIButton : public UButton
{
	GENERATED_BODY()
	
public:

	UUIButton();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIIndex")
		FVector2D UIIndex = FVector2D(0.0f, 0.0f);
};
