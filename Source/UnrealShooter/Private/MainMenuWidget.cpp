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
	DataInstance->OnUINavigation.AddDynamic(this, &UMainMenuWidget::UINavigate);
	DataInstance->OnUISelection.AddDynamic(this, &UMainMenuWidget::UIClicked);
}

void UMainMenuWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	UINativeNavigation();
}

void UMainMenuWidget::UIClicked_Implementation()
{
}

void UMainMenuWidget::UINavigate_Implementation(FVector2D direction)
{
}

void UMainMenuWidget::SetHoverState(TArray<UButton*> buttons)
{
	for (auto& btn : buttons)
	{
		if (btn->HasKeyboardFocus())
		{
			btn->SetBackgroundColor(HOVER_STATE_COLOR);
		}
		else
		{
			btn->SetBackgroundColor(NORMAL_STATE_COLOR);
		}
	}
}

void UMainMenuWidget::UINativeNavigation()
{
	//FVector2D currentIndex = UIIndex;
	//UIIndex += !bIsNavigationActive ? FVector2D(0.0f,0.0f) : direction;
	//CapLowUIIndexValue();

	if (EMainMenuVisible == ESlateVisibility::Visible)
	{
		//CapUIIndexValue(wMainMenuButtons);
		//SetMainState(wMainMenuButtons);
		SetHoverState(wMainMenuButtons);
	}
	else if (EOptionsMenuVisible == ESlateVisibility::Visible)
	{
		//CapUIIndexValue(wOptionsMenuButtons);
		//SetMainState(wOptionsMenuButtons);
		SetHoverState(wOptionsMenuButtons);
	}
	else if (ECreditsWindowVisible == ESlateVisibility::Visible)
	{
		SetHoverState(wCreditsWindowButtons);
	}

	//bIsNavigationActive = true;
}

bool UMainMenuWidget::SupportKeyboardFocus()
{
	//on click makes the current element loose the focus :S
	/*for (auto& btn : wMainMenuButtons)
	{
		if (btn->HasKeyboardFocus())
		{
			return true;
		}
	}
	for (auto& btn : wOptionsMenuButtons)
	{
		if (btn->HasKeyboardFocus())
		{
			return true;
		}
	}*/
	return false;
}

void UMainMenuWidget::ExecuteConsoleCommand(FString cmd)
{
	if (PController)
	{
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

#pragma region Unused
/*void UMainMenuWidget::SetMainState(TArray<UButton*> buttons)
{
for (auto& btn : buttons)
{
btn->SetBackgroundColor(NORMAL_STATE_COLOR);
}
}*/

/*void UMainMenuWidget::ClickButton(TArray<UButton*> buttons)
{
for (auto& btn : buttons)
{
if (UIIndex == btn->UIIndex)
{
//btn
//TODO epombo: how do I know which function to call??
}
}
}*/

/*void UMainMenuWidget::CapUIIndexValue(TArray<UButton*> buttons)
{
for (int32 i = UIIndex.X; i > 0; --i)
{
for (auto& btn : buttons)
{
if (i == btn->UIIndex.X && UIIndex.Y == btn->UIIndex.Y)
{
UIIndex = FVector2D(i, UIIndex.Y);
}
}
}
}*/

/*void UMainMenuWidget::PlayButtonPressed_test()
{
UGameplayStatics::OpenLevel(this, "UnrealtrainingGrounds");
PController->bShowMouseCursor = false;
this->RemoveFromParent();
FInputModeGameOnly Mode;
PController->SetInputMode(Mode);
}*/

/*void UMainMenuWidget::CapLowUIIndexValue()
{
if (UIIndex.X < 0.0f)
{
UIIndex.X = 0.0f;
}
if (UIIndex.Y < 0.0f)
{
UIIndex.Y = 0.0f;
}
}*/

#pragma endregion

