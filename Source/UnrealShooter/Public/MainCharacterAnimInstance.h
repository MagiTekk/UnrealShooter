// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "MainCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UNREALSHOOTER_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UAnimMontage* Equip_Pistol_Montage;
	UAnimMontage* Unequip_Pistol_Montage;
	UAnimMontage* Reload_Pistol_Montage;
	USoundCue* Reload_SoundCue;
	UAnimSequence* Recoil_Sequence;
	bool bIsReloading;

	const FRotator INITIAL_ARMS_ROTATION = FRotator(0,0,0);

	void InitMontageAnimations();

	//void UpdateArmsRotation();	//unused, but may help for a future bone rotation

public:
	UMainCharacterAnimInstance();

	UPROPERTY()
		bool bEquipPistolAnimation;

	/** Movement Speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		bool bIsPistolEquipped;

	/** Arms Rotation, Set in MainCharacter.cpp Tick */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim")
		FRotator SkelControl_UpperArmstRotation;

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void NotifyCallback_PistolEquipped();

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void NotifyCallback_WeaponReloaded();

	UFUNCTION()
		void AttachOrDetachWeapon();

	UFUNCTION()
		void RecoilAnimation();

	UFUNCTION()
		void PlayStepSound(USoundCue* Step_SoundCue);

	UFUNCTION()
		void EquipPistol();

	UFUNCTION()
		void UnequipPistol();
	
	UFUNCTION()
		void ReloadPistol();

//init and tick
public:

	APawn * OwningPawn;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

	void ResetArmsRotation();
};
