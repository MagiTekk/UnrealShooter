// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UMG.h"
#include "Components/Button.h"
#include "SequenceSelectorWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API USequenceSelectorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//OTHER PROPERTIES
	const FLinearColor NORMAL_STATE_COLOR = FLinearColor(0.536f, 1.0f, 0.0f, 1.0f);
	const FLinearColor HOVER_STATE_COLOR = FLinearColor(1.0f, 0.09f, 0.0f, 1.0f);
	const FLinearColor PRESSED_STATE_COLOR = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);

	virtual void NativeConstruct() override;

	// Called every frame
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	//OTHER PROPERTIES
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UIButtons")
		TArray<UButton*> wSequenceSelectorButtons;

	//UI NAV
	UFUNCTION(BlueprintNativeEvent, Category = "UINavigation")
		void UIClicked();

	UFUNCTION(BlueprintNativeEvent, Category = "UINavigation")
		void UINavigate(FVector2D direction);

	UFUNCTION(Category = "UINavigation")
		void SetHoverState(TArray<UButton*> buttons);

	UFUNCTION(Category = "UINavigation")
		void UINativeNavigation();

	UFUNCTION(BlueprintCallable, Category = "UIAction")
		void UISequenceSelected(ESequenceEnum sequenceType);

	UFUNCTION(BlueprintCallable, Category = "UIAction")
		void OnCancel();
	
	
};
