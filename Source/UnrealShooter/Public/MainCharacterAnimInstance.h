// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
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

	void InitMontageAnimations()
	{
		//you can only get the anim montages(also maybe BPs?) using the constructor helper from the constructor
		ConstructorHelpers::FObjectFinder<UAnimMontage> FO_EquipPistolMontage(TEXT("AnimMontage'/Game/UnrealShooter/Animation/MainCharacter/Actions/Equip_Pistol_Montage.Equip_Pistol_Montage'"));
		Equip_Pistol_Montage = FO_EquipPistolMontage.Object;

		ConstructorHelpers::FObjectFinder<UAnimMontage> FO_UnequipPistolMontage(TEXT("AnimMontage'/Game/UnrealShooter/Animation/MainCharacter/Actions/Unequip_Pistol_Montage.Unequip_Pistol_Montage'"));
		Unequip_Pistol_Montage = FO_UnequipPistolMontage.Object;

		ConstructorHelpers::FObjectFinder<UAnimMontage> FO_ReloadPistolMontage(TEXT("AnimMontage'/Game/UnrealShooter/Animation/MainCharacter/Actions/Reload_Pistol_Montage.Reload_Pistol_Montage'"));
		Reload_Pistol_Montage = FO_ReloadPistolMontage.Object;

		ConstructorHelpers::FObjectFinder<USoundCue> Reload_Cue(TEXT("SoundCue'/Game/UnrealShooter/Audio/Eject_Clip_And_Re-Load_Cue.Eject_Clip_And_Re-Load_Cue'"));
		Reload_SoundCue = Reload_Cue.Object;

		ConstructorHelpers::FObjectFinder<UAnimSequence> FO_RecoilAnimSequence(TEXT("AnimSequence'/Game/UnrealShooter/Animation/MainCharacter/Actions/Shoot_Pistol_Recoil.Shoot_Pistol_Recoil'"));
		Recoil_Sequence = FO_RecoilAnimSequence.Object;
	}

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
