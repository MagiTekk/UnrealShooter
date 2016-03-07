// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasicButton.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class UNREALSHOOTER_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* ThirdPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bEquipPistol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bUnEquipPistol;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	bool bReloadPistol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bCameraZoomIn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bCameraZoomOut;

#pragma region Constants

protected:

	const FRotator INITIAL_ROTATION = FRotator(0, 0, 0);
	const float SPEED_RUNNING = 0.8f;
	const float SPEED_WALKING = 0.45f;
	const float TURN_BACK_RATE = 2.0f;
	const float TURN_BACK_COMPLETE_BUILDUP = 73.0f;
	const float BASE_AIM_AROUND_RATE = 150.0f;
	const float BASE_LOOK_AROUND_RATE = 300.0f;
	const float ROTATION_BOUNDS_BASE_VALUE = 70.f;
	const float AIM_BOUNDS_PITCH_VALUE = 90.f;
	const float AIM_BOUNDS_YAW_VALUE = 30.f;
	const float BASE_CAMERA_ARM_LEGNTH = 115.f;
	const float BASE_CAMERA_ZOOMED_ARM_LEGNTH = 60.f;
	const float CAMERA_BRANG_CLAMP_RATE = 10.0f;
	const float ZOOM_RATE = 1.5f;

#pragma endregion
	
#pragma region Camera

protected:

	void initCameraComponents();

	bool bBrangCameraPitch;
	bool bBrangCameraYaw;
	bool bBrangCharacterYaw;

	void BRangCameraPitch();
	void BRangCameraYaw();
	void BRangCharacterYaw();

	/* called to turn the camera too look left or right */
	void LookRight(float rate);
	void AimRight(float rate);
	void CameraLookRight(float rate);

	/* called to turn the camera too look up or down */
	void LookUp(float rate);
	void AimUp(float rate);
	void CameraLookUp(float rate);

#pragma endregion

#pragma region Movement

public:

	// current speed value given by the controller
	bool bIsWalkingBackwards;

protected:

	FRotator FinalRotation_Character_Rotation;
	bool bIsRunning;
	bool bTurnBack;
	float turnBackBuildUp;

	/** Called for forwards/backward input */
	void MoveForward(float value);

	/** Called for side to side input */
	void MoveRight(float value);
	void AddYawRotation(float value, bool forceMove = false);
	void Run();
	void DoNotRun();
	void TurnBack();

#pragma endregion

#pragma region Animations

protected:

	/** Equip and unequip pistol functions*/
	void Equip_Pistol();

	/** Aim camera animations */
	void Trigger_Aim_In();
	void Trigger_Aim_Out();
	void Aim_In();
	void Aim_Out();
	void ReloadWeapon();
	void ShootWeapon();
	void RecoilAnimation();
	void ExecuteContextAction();

#pragma endregion

#pragma region Weapon

protected:

	AActor* ActiveWeapon;

public:

	void AttachOrDetachWeapon();

#pragma endregion

#pragma region Sound

private:
	USoundCue* TurnBack_SoundCue;

#pragma endregion

#pragma region IteractiveActor
	/* Listerner to Action Events */
	UPROPERTY()
		ABasicButton* ActionListener;

	UFUNCTION()
	void OnRegisterActorAsListener(AActor* IteractiveActor);

	UFUNCTION()
	void OnUnregisterActorAsListener();
	
#pragma endregion
};
