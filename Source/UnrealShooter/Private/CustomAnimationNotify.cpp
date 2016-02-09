// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "CustomAnimationNotify.h"

UCustomAnimationNotify::UCustomAnimationNotify()
{
	ConstructorHelpers::FObjectFinder<USoundCue> StepSound_Cue(TEXT("SoundCue'/Game/UnrealShooter/Audio/WalkStepSound_Cue.WalkStepSound_Cue'"));
	Step_SoundCue = StepSound_Cue.Object;
}

void UCustomAnimationNotify::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	switch (ENotifyCustomName)
	{
		case ECustomName::PistolEquippedNotify:
		case ECustomName::PistolUnequippedNotify:
			if (UCustomAnimationNotify::getMainCharAnimInstance(MeshComp))
			{
				UCustomAnimationNotify::getMainCharAnimInstance(MeshComp)->NotifyCallback_PistolEquipped();
			}
			break;
		case ECustomName::PistolReloadedNotify:
			break;
		case ECustomName::StepSoundNotify:
			if (UCustomAnimationNotify::getMainCharAnimInstance(MeshComp))
			{
				UCustomAnimationNotify::getMainCharAnimInstance(MeshComp)->PlayStepSound(Step_SoundCue);
			}
			break;
		default:
			break;
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *animName);
	//GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Yellow, FString::FString("STEP_SOUND"));
}

UMainCharacterAnimInstance * UCustomAnimationNotify::getMainCharAnimInstance(USkeletalMeshComponent * MeshComp)
{
	if (!MeshComp) return nullptr;
	UMainCharacterAnimInstance * AnimationInst = Cast<UMainCharacterAnimInstance>(MeshComp->GetAnimInstance());
	if (!AnimationInst) return nullptr;
	return AnimationInst;
}