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
 * UIButton is an extension of UButton only that it contains an index,
 * with this index you can control the way we navigate the widget according
 * to a gamepad or keyboard input
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
