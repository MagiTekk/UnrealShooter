// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UMG.h"
#include "Components/Button.h"
#include "FinisherWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API UFinisherWidget : public UUserWidget
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UIBindings")
		int32 Points;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UIBindings")
		int32 AmountNormalHits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UIBindings")
		int32 AmountHeadshotHits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UIBindings")
		int32 AmountBonusHits;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UIBindings")
		FName CurrentSequenceName;

	//OTHER PROPERTIES
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UIButtons")
		TArray<UButton*> wFinisherButtons;

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
		void OnConfirm();
	
};
