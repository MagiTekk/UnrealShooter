// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "MainCharacter.h"
#include "Weapon_M9.h"
#include "UnrealShooterDataSingleton.h"
#include "HUDUserWidget.h"


void UHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHUDUserWidget::InitWidget()
{
	MyCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UpdateHUD();

	//event handler
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnWeaponShot.AddDynamic(this, &UHUDUserWidget::UpdateHUD);
	DataInstance->OnWeaponReloaded.AddDynamic(this, &UHUDUserWidget::UpdateHUD);
	DataInstance->OnWeaponEquipped.AddDynamic(this, &UHUDUserWidget::UpdateHUD);
}

void UHUDUserWidget::UpdateWeaponAmmoAvailable()
{
	AWeapon_M9* Weapon = Cast<AWeapon_M9>(MyCharacter->ActiveWeapon);
	WeaponAmmoAvailable = Weapon ? Weapon->Ammo : 0;
}

void UHUDUserWidget::UpdateWeaponAmmoCapacity()
{
	AWeapon_M9* Weapon = Cast<AWeapon_M9>(MyCharacter->ActiveWeapon);
	WeaponAmmoCapacity = Weapon ? Weapon->AmmoCapacity : 0;
}

void UHUDUserWidget::UpdateAmmoStored()
{
	AmmoStored = MyCharacter->AmmoAvailable;
}

void UHUDUserWidget::UpdateHUD()
{
	UpdateWeaponAmmoAvailable();
	UpdateWeaponAmmoCapacity();
	UpdateAmmoStored();
}
