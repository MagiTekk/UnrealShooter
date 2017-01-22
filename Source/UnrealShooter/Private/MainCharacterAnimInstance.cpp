// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "MainCharacterAnimInstance.h"
#include "MainCharacter.h"
#include "Weapon_M9.h"

//Constructor
UMainCharacterAnimInstance::UMainCharacterAnimInstance()
{
	UMainCharacterAnimInstance::InitMontageAnimations();
}

//This function is like PostInitAnimtree in UE3
void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	//Very Important Line
	Super::NativeInitializeAnimation();

	//Cache the owning pawn for use in Tick
	OwningPawn = TryGetPawnOwner();
}

//Tick
void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	//Very Important Line
	Super::NativeUpdateAnimation(DeltaTimeX);

	//Always Check Pointers
	if (!OwningPawn)
	{
		return;
	}

	//Set movement speed
	MovementSpeed = OwningPawn->GetVelocity().Size();
	//bIsMoving = (OwningPawn->GetVelocity().SizeSquared() > 25);

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OwningPawn);
	MovementSpeed = MovementSpeed * (MainCharacter->bIsWalkingBackwards ? -1 : 1);

	//cast just once into a single var for performance
	bool bMainCharEquipPistol = MainCharacter->bEquipPistol;

	//** Equip/Unequip pistol animations
	if (bMainCharEquipPistol && !bEquipPistolAnimation)
	{
		bEquipPistolAnimation = true;
		UMainCharacterAnimInstance::EquipPistol();
	}
	else if (!bMainCharEquipPistol && bEquipPistolAnimation)
	{
		bEquipPistolAnimation = false;
		UMainCharacterAnimInstance::UnequipPistol();
	}

	//** Reload pistol animation
	if (MainCharacter->bReloadPistol && !bIsReloading)
	{
		bIsReloading = true;
		UMainCharacterAnimInstance::ReloadPistol();
	}
}

void UMainCharacterAnimInstance::EquipPistol()
{
	if (!Montage_IsPlaying(Equip_Pistol_Montage))
	{
		Montage_Play(Equip_Pistol_Montage);
	}
}

/** Notify event, called by custom native definition */
void UMainCharacterAnimInstance::NotifyCallback_PistolEquipped()
{
	bIsPistolEquipped = !bIsPistolEquipped;
	UMainCharacterAnimInstance::AttachOrDetachWeapon();
}

void UMainCharacterAnimInstance::NotifyCallback_WeaponReloaded()
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OwningPawn);
	MainCharacter->ReloadWeapon();
	MainCharacter->bReloadPistol = false;
	bIsReloading = false;
}

void UMainCharacterAnimInstance::UnequipPistol()
{
	if (!Montage_IsPlaying(Unequip_Pistol_Montage))
	{
		Montage_Play(Unequip_Pistol_Montage);
	}
}

//TODO nullBot: add a parameter, enum maybe for weapon types
void UMainCharacterAnimInstance::AttachOrDetachWeapon()
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OwningPawn);
	MainCharacter->AttachOrDetachWeapon();
}

void UMainCharacterAnimInstance::ReloadPistol()
{
	if (!Montage_IsPlaying(Reload_Pistol_Montage))
	{
		Montage_Play(Reload_Pistol_Montage);
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OwningPawn);
		UGameplayStatics::PlaySoundAtLocation(this, Reload_SoundCue, MainCharacter->GetActorLocation());
	}
}

//TODO nullBot: recoil animation, it's all connected already, just find an animation that works
void UMainCharacterAnimInstance::RecoilAnimation()
{
	//PlaySlotAnimation(Recoil_Sequence, "UpperBody");

	/*if (!IsPlayingSlotAnimation(Recoil_Sequence, "UpperBody"))
	{
		PlaySlotAnimation(Recoil_Sequence, "UpperBody");
	}*/
}

void UMainCharacterAnimInstance::PlayStepSound(USoundCue * Step_SoundCue)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OwningPawn);
	UGameplayStatics::PlaySoundAtLocation(this, Step_SoundCue, MainCharacter->GetActorLocation());
}

void UMainCharacterAnimInstance::ResetArmsRotation()
{
	SkelControl_UpperArmstRotation = INITIAL_ARMS_ROTATION;
}

#pragma region Unused & Useful Code

//** Get the upperLeft and UpperRight arm bones and apply the rotation
/*void UMainCharacterAnimInstance::UpdateArmsRotation()
{
	//	"upperarm_l"/"upperarm_r"/"lowerarm_l"/"lowerarm_r"

	if (SkelControl_UpperArmstRotation.Yaw > 0.0f)
	{
		auto upperarm_l_Idx = GetSkelMeshComponent()->GetBoneIndex(TEXT("lowerarm_l"));
		auto upperarm_r_Idx = GetSkelMeshComponent()->GetBoneIndex(TEXT("lowerarm_r"));

		USkeletalMeshComponent* SkeletalMeshComponent = GetSkelMeshComponent();
		FQuat upperL_Rotation = SkeletalMeshComponent->LocalAtoms[upperarm_l_Idx].GetRotation();
		FQuat upperR_Rotation = SkeletalMeshComponent->LocalAtoms[upperarm_r_Idx].GetRotation();

		SkeletalMeshComponent->LocalAtoms[upperarm_l_Idx].SetRotation(SkelControl_UpperArmstRotation.Quaternion() + upperL_Rotation);
		SkeletalMeshComponent->LocalAtoms[upperarm_r_Idx].SetRotation(SkelControl_UpperArmstRotation.Quaternion() + upperR_Rotation);
	}
	

	//GetSkelMeshComponent()->FillSpaceBases(CurrentSkeleton, );

	//auto BoneIdx = SkeletalMeshComponent->GetBoneIndex(TEXT("SheKRArm2"));
	//SkeletalMeshComponent->LocalAtoms[BoneIdx].AddToTranslation(FVector(100, 100, 100));
	//SkeletalMeshComponent->FillSpaceBases();
}

*/

/*
// Get Current Montage Position
float Montage_GetPosition(UAnimMontage* Montage);
// Has Montage been stopped?
bool Montage_GetIsStopped(UAnimMontage* Montage);

// Set position.
void Montage_SetPosition(UAnimMontage* Montage, float NewPosition);

// Get PlayRate
float Montage_GetPlayRate(UAnimMontage* Montage);

// Get next sectionID for given section ID
int32 Montage_GetNextSectionID(UAnimMontage* Montage, int32 CurrentSectionID);

// Get Current Active Montage in this AnimInstance
UAnimMontage * GetCurrentActiveMontage();

// Get Currently active montage instace
FAnimMontageInstance* GetActiveMontageInstance();


#include <iostream>       // std::cout
#include <typeinfo>       // std::bad_cast
try
{
AMainCharacter* MainCharacter = Cast<AMainCharacter>(OwningPawn);
}
catch (std::bad_cast& bc)
{
std::cerr << "bad_cast caught: " << bc.what() << '\n';
return;
}

//ITERATE ARRAY
TArray<FScalarParameterValue> scalarParams = materialInst->ScalarParameterValues;
for (FScalarParameterValue param : scalarParams)
{
if (param.ParameterName == "OpacityModifier")
{
opacityVal -= 0.1;
materialInst->SetScalarParameterValue()
param.ParameterValue = opacityVal;
}
}

//Apply the skin to all skin elements
Skin = UMaterialInstanceDynamic::Create(SkinMaterial, this);
if (Skin)
{
for (auto Iter(SkinElements.CreateIterator()); Iter; Iter++)
{
Mesh->SetMaterial(*Iter, Skin);
}
}

*/

#pragma endregion


