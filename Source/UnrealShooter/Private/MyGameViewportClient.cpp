// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "MyGameViewportClient.h"




TOptional<bool> UMyGameViewportClient::QueryShowFocus(const EFocusCause InFocusCause) const
{
	return false;
}