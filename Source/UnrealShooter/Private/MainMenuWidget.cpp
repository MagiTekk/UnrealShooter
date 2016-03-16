// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include <string>
#include "MainMenuWidget.h"


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void UMainMenuWidget::ExecuteConsoleCommand(FString cmd)
{
	if (PController)
	{
		//PController->ConsoleCommand(TEXT("stat fps"), true);
		PController->ConsoleCommand(*cmd, true);
	}
}

void UMainMenuWidget::SetQuality(int32 value)
{
	switch (value)
	{
	case 0:
		ExecuteConsoleCommand(RESOLUTION_LOW);
		ExecuteConsoleCommand(VIEWDISTANCE_LOW);
		ExecuteConsoleCommand(ANTIALIASING_LOW);
		ExecuteConsoleCommand(POSTPROCESSING_LOW);
		ExecuteConsoleCommand(SHADOWS_LOW);
		ExecuteConsoleCommand(TEXTURES_LOW);
		ExecuteConsoleCommand(EFFECTS_LOW);
		break;
	case 1:
		ExecuteConsoleCommand(RESOLUTION_MID);
		ExecuteConsoleCommand(VIEWDISTANCE_MID);
		ExecuteConsoleCommand(ANTIALIASING_MID);
		ExecuteConsoleCommand(POSTPROCESSING_MID);
		ExecuteConsoleCommand(SHADOWS_MID);
		ExecuteConsoleCommand(TEXTURES_MID);
		ExecuteConsoleCommand(EFFECTS_MID);
		break;
	case 2:
		ExecuteConsoleCommand(RESOLUTION_HIGH);
		ExecuteConsoleCommand(VIEWDISTANCE_HIGH);
		ExecuteConsoleCommand(ANTIALIASING_HIGH);
		ExecuteConsoleCommand(POSTPROCESSING_HIGH);
		ExecuteConsoleCommand(SHADOWS_HIGH);
		ExecuteConsoleCommand(TEXTURES_HIGH);
		ExecuteConsoleCommand(EFFECTS_HIGH);
		break;
	case 3:
		ExecuteConsoleCommand(RESOLUTION_EPIC);
		ExecuteConsoleCommand(VIEWDISTANCE_EPIC);
		ExecuteConsoleCommand(ANTIALIASING_EPIC);
		ExecuteConsoleCommand(POSTPROCESSING_EPIC);
		ExecuteConsoleCommand(SHADOWS_EPIC);
		ExecuteConsoleCommand(TEXTURES_EPIC);
		ExecuteConsoleCommand(EFFECTS_EPIC);
		break;
	}
}

void UMainMenuWidget::OnSliderValueChanged(float value)
{
	value *= 100;
	ExecuteConsoleCommand("sg.ResolutionQuality " + FString::SanitizeFloat(value));
}

void UMainMenuWidget::ShowMouseCursor(bool value)
{
	if (PController)
	{
		PController->bShowMouseCursor = value;
	}
}

