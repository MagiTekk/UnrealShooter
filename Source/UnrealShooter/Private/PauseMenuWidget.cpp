// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "UnrealShooterDataSingleton.h"
#include "PauseMenuWidget.h"



void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnUINavigation.AddDynamic(this, &UPauseMenuWidget::UINavigate);
	DataInstance->OnUISelection.AddDynamic(this, &UPauseMenuWidget::UIClicked);
}

void UPauseMenuWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	UINativeNavigation();
}

void UPauseMenuWidget::UIClicked_Implementation()
{
}

void UPauseMenuWidget::UINavigate_Implementation(FVector2D direction)
{
}

void UPauseMenuWidget::SetHoverState(TArray<UButton*> buttons)
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

void UPauseMenuWidget::UINativeNavigation()
{
	SetHoverState(wPauseMenuButtons);
}
