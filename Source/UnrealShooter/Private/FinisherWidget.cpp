// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "FinisherWidget.h"
#include "BasicButton.h"
#include "UnrealShooterDataSingleton.h"
#include "UnrealShooterLevelScriptActor.h"

void UFinisherWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AUnrealShooterLevelScriptActor* MyLvlBP = Cast<AUnrealShooterLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (MyLvlBP)
	{
		Points = MyLvlBP->Points;
		AmountNormalHits = MyLvlBP->AmountNormalHits;
		AmountHeadshotHits = MyLvlBP->AmountHeadshotHits;
		AmountBonusHits = MyLvlBP->AmountBonusHits;
		CurrentSequenceName = MyLvlBP->CurrentSequenceName;
	}

	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnUINavigation.AddDynamic(this, &UFinisherWidget::UINavigate);
	DataInstance->OnUISelection.AddDynamic(this, &UFinisherWidget::UIClicked);
}

void UFinisherWidget::NativeTick(const FGeometry & MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	UINativeNavigation();
}

void UFinisherWidget::UIClicked_Implementation()
{
}

void UFinisherWidget::UINavigate_Implementation(FVector2D direction)
{
}

void UFinisherWidget::SetHoverState(TArray<UButton*> buttons)
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

void UFinisherWidget::UINativeNavigation()
{
	SetHoverState(wFinisherButtons);
}

void UFinisherWidget::OnConfirm()
{
	for (TActorIterator<ABasicButton> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ABasicButton *Button = *ActorItr;
		if (Button->GetName() == "SequenceButton")
		{
			Button->ActivateButton();
			break;
		}
	}
}


