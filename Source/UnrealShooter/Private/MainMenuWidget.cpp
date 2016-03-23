// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include <string>
#include "UnrealShooterDataSingleton.h"
#include "MainMenuWidget.h"


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnUINavigation.AddDynamic(this, &UMainMenuWidget::UINavigate_Callback);
	DataInstance->OnUISelection.AddDynamic(this, &UMainMenuWidget::UIClicked);
}

void UMainMenuWidget::UINavigate_Implementation()
{
}

void UMainMenuWidget::UIClicked_Implementation()
{
}

void UMainMenuWidget::SetMainStateForButtons(TArray<UButton*> buttons)
{
	for (auto& btn : buttons)
	{
		btn->SetBackgroundColor(NORMAL_STATE_COLOR);
	}
}

void UMainMenuWidget::SetHoverStateForMainButtons()
{
	int32 val = UIIndex.Y;
	UButton* btn = wMainMenuButtons[val];
	btn->SetBackgroundColor(HOVER_STATE_COLOR);
}

void UMainMenuWidget::CapLowUIIndexValue()
{
	if (UIIndex.X < 0.0f)
	{
		UIIndex.X = 0.0f;
	}
	if (UIIndex.Y < 0.0f)
	{
		UIIndex.Y = 0.0f;
	}
}

void UMainMenuWidget::UINavigate_Callback(FVector2D direction)
{
	UIIndex += direction;
	CapLowUIIndexValue();

	if (bIsMainMenuVisible)
	{
		CapUIIndexValue(0, 2);
		SetMainStateForButtons(wMainMenuButtons);
		SetHoverStateForMainButtons();
	}

	//call the blueprint native event
	//UMainMenuWidget::UINavigate();
}

void UMainMenuWidget::CapUIIndexValue(int32 Xcap, int32 Ycap)
{
	UIIndex.X = UIIndex.X > Xcap ? Xcap : UIIndex.X;
	UIIndex.Y = UIIndex.Y > Ycap ? Ycap : UIIndex.Y;
}

void UMainMenuWidget::ExecuteConsoleCommand(FString cmd)
{
	if (PController)
	{
		//PController->ConsoleCommand(TEXT("stat fps"), true);
		//PController->ConsoleCommand(TEXT("stat unit"), true);
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

