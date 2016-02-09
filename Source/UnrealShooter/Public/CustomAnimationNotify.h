// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "MainCharacterAnimInstance.h"
#include "CustomAnimationNotify.generated.h"

UENUM()
enum class ECustomName : uint8
{
	PistolEquippedNotify,	//UMETA(DisplayName="PistolEquippedNotify"),
	PistolUnequippedNotify,	//UMETA(DisplayName = "PistolUnequippedNotify"),
	PistolReloadedNotify,	//UMETA(DisplayName = "PistolReloadedNotify")
	StepSoundNotify
};

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API UCustomAnimationNotify : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UMainCharacterAnimInstance* getMainCharAnimInstance(USkeletalMeshComponent * MeshComp);

public:

	UCustomAnimationNotify();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimNotify)
	ECustomName ENotifyCustomName;

#pragma region Sound

private:
	USoundCue* Step_SoundCue;

#pragma endregion
};