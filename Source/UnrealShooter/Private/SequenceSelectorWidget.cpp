// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "BasicButton.h"
#include "UnrealShooterDataSingleton.h"
#include "UnrealShooterLevelScriptActor.h"
#include "SequenceSelectorWidget.h"

void USequenceSelectorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnUINavigation.AddDynamic(this, &USequenceSelectorWidget::UINavigate);
	DataInstance->OnUISelection.AddDynamic(this, &USequenceSelectorWidget::UIClicked);
}

void USequenceSelectorWidget::NativeTick(const FGeometry & MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	UINativeNavigation();
}

void USequenceSelectorWidget::UISequenceSelected(ESequenceEnum sequenceType)
{
	AUnrealShooterLevelScriptActor* MyLvlBP = Cast<AUnrealShooterLevelScriptActor>(GetWorld()->GetLevelScriptActor());

	if (!MyLvlBP) return;
	MyLvlBP->PlaySequence(sequenceType);
}

void USequenceSelectorWidget::UIClicked_Implementation()
{
}

void USequenceSelectorWidget::UINavigate_Implementation(FVector2D direction)
{
}

void USequenceSelectorWidget::SetHoverState(TArray<UButton*> buttons)
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

void USequenceSelectorWidget::UINativeNavigation()
{
	SetHoverState(wSequenceSelectorButtons);
}

void USequenceSelectorWidget::OnCancel()
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
