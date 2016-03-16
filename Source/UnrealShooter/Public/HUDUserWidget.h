// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "HUDUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API UHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void InitWidget();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
		class AMainCharacter* MyCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		int32 WeaponAmmoAvailable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		int32 WeaponAmmoCapacity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
		int32 AmmoStored;

	UFUNCTION(Category = "Bindings")
		void UpdateWeaponAmmoAvailable();

	UFUNCTION(Category = "Bindings")
		void UpdateWeaponAmmoCapacity();

	UFUNCTION(Category = "Bindings")
		void UpdateAmmoStored();

	UFUNCTION(Category = "Bindings")
		void UpdateHUD();
	
};
