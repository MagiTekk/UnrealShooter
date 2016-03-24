// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameViewportClient.h"
#include "MyGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API UMyGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:

	//overriding this method you can activate the dotted line in keyboard focus or not
	virtual TOptional<bool> QueryShowFocus(const EFocusCause InFocusCause) const override;
	
	
};
