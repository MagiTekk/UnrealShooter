// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "MainCharacterAnimInstance.h"
#include "CustomAnimationNotify.generated.h"

//UENUM(BluePrintType)
enum class ECustomName:uint8
{
	PistolEquippedNotify,	//UMETA(DisplayName="PistolEquippedNotify"),
	PistolUnequippedNotify,	//UMETA(DisplayName = "PistolUnequippedNotify"),
	PistolReloadedNotify,	//UMETA(DisplayName = "PistolReloadedNotify")
	StepSoundNotify,
	CustomNameMismatch
};

/**
 * 
 */
UCLASS()
class UNREALSHOOTER_API UCustomAnimationNotify : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	ECustomName getNameEnum(FString const& inString);

	UMainCharacterAnimInstance* getMainCharAnimInstance(USkeletalMeshComponent * MeshComp);

public:

	UCustomAnimationNotify();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimNotify)
	FString NotifyCustomName;

#pragma region Sound

private:
	USoundCue* Step_SoundCue;

#pragma endregion
};