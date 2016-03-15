// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "MainCharacter.h"
#include "TimelineHelper.h"
#include "MainCharacterAnimInstance.h"
#include "Weapon_M9.h"
#include "UnrealShooterLevelScriptActor.h"
#include "UnrealShooterDataSingleton.h"
#include "RotatableTarget.h"


/** 
Sets default values
*/
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	//we want the controller to rotate the camera's Yaw
	//bUseControllerRotationYaw = 0;

	bUseControllerRotationPitch = 0;
	bUseControllerRotationRoll = 0;

	initCameraComponents();

	ConstructorHelpers::FObjectFinder<USoundCue> TurnBack_Cue(TEXT("SoundCue'/Game/UnrealShooter/Audio/TurnBackSound_Cue.TurnBackSound_Cue'"));
	TurnBack_SoundCue = TurnBack_Cue.Object;

	InitBasicValues();
}

void AMainCharacter::InitBasicValues()
{
	//initial player values
	Health = 100.0f;
	Stamina = 100.0f;
	AmmoAvailable = 60;
}

/**
Called when the game starts or when spawned
*/
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	bBrangCameraPitch = false;
	bBrangCameraYaw = false;
	bBrangCharacterYaw = false;
	bEquipPistol = false;
	bUnEquipPistol = false;
	bCameraZoomIn = false;
	bCameraZoomOut = false;
	bIsWalkingBackwards = false;
	bIsRunning = false;
	bTurnBack = false;

	//delegate handler
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnActorBeginOverlap.AddDynamic(this, &AMainCharacter::OnRegisterActorAsListener);
	DataInstance->OnActorEndOverlap.AddDynamic(this, &AMainCharacter::OnUnregisterActorAsListener);

	//by default have the pistol equipped
	MakePistolEquipped();

	ActionListener = nullptr;
}

void AMainCharacter::initCameraComponents()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character

	// Create a follow camera
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	ThirdPersonCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

/** 
Called every frame
*/
void AMainCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bBrangCameraPitch)
	{
		BRangCameraPitch();
	}

	if (bBrangCameraYaw)
	{
		BRangCameraYaw();
	}

	if (bCameraZoomIn)
	{
		Aim_In();
	}

	if (bCameraZoomOut)
	{
		Aim_Out();
	}

	if (bBrangCharacterYaw)
	{
		BRangCharacterYaw();
	}

	if (bTurnBack)
	{
		TurnBack();
	}
}

/**
Called to bind functionality to input
*/
void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	/*Movement Axes*/
	InputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	/*Turn Axes Keys*/
	InputComponent->BindAxis("LookRight", this, &AMainCharacter::LookRight);
	InputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);

	/*Turn Axes Mouse*/
	//Another set of configuration is needed to play with the mouse, the camera system has to be redefined.
	//InputComponent->BindAxis("MouseLookRight", this, &APawn::AddControllerYawInput);
	//InputComponent->BindAxis("MouseLookUp", this, &AMainCharacter::LookUp);

	/*Action Key Mapping*/
	InputComponent->BindAction("Equip", IE_Pressed, this, &AMainCharacter::Equip_Pistol);

	InputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::Trigger_Aim_In);
	InputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::Trigger_Aim_Out);

	InputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacter::StartReloading);

	InputComponent->BindAction("Shoot", IE_Pressed, this, &AMainCharacter::ShootWeapon);

	InputComponent->BindAction("Run", IE_Pressed, this, &AMainCharacter::Run);
	InputComponent->BindAction("Run", IE_Released, this, &AMainCharacter::DoNotRun);

	InputComponent->BindAction("ExecuteAction", IE_Pressed, this, &AMainCharacter::ExecuteContextAction);

	InputComponent->BindAction("Pause", IE_Pressed, this, &AMainCharacter::PauseGame);
}

void AMainCharacter::MoveForward(float value)
{
	if ((Controller != NULL) && (value != 0.0f) && !bCameraZoomIn && !bTurnBack)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector forwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const float TopSpeed = bIsRunning ? SPEED_RUNNING : SPEED_WALKING;

		// clamp the speed value so that we walk when going backwards
		float clampedVal = FMath::Clamp(value, -0.3f, TopSpeed);

		bIsWalkingBackwards = value < 0;

		//turn around
		if (bIsRunning && bIsWalkingBackwards && !bTurnBack)
		{
			turnBackBuildUp = 0.0f;
			bTurnBack = true;
			UGameplayStatics::PlaySoundAtLocation(this, TurnBack_SoundCue, GetActorLocation());
		}
		else
		{
			AddMovementInput(forwardVector, clampedVal);
		}
	}
}

/**
We rotate instead of move with the move right function
*/
void AMainCharacter::MoveRight(float value)
{
	AddYawRotation(value * 0.8f);
}

void AMainCharacter::TurnBack()
{
	turnBackBuildUp += TURN_BACK_RATE;

	if (turnBackBuildUp < TURN_BACK_COMPLETE_BUILDUP)
	{
		AddYawRotation(TURN_BACK_RATE);
	}
	else
	{
		turnBackBuildUp = 0.0f;
		bTurnBack = false;
	}
}

void AMainCharacter::Run()
{
	bIsRunning = true;
}

void AMainCharacter::DoNotRun()
{
	bIsRunning = false;
}

/**	
#forceMove if true will rotate our character even if we are aiming 
*/
void AMainCharacter::AddYawRotation(float value, bool forceMove)
{
	if ((Controller != NULL) && (value != 0.0f) && (!bCameraZoomIn || forceMove))
	{
		AddControllerYawInput(value);
	}
}

/**
We cannot use the controller YawInput for this one since it will also rotate the Pawn
Instead we have to rotate directly on the Camera String Component
*/
void AMainCharacter::LookRight(float rate)
{
	if (rate != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		//in this case we are aiming
		if (bCameraZoomIn)
		{
			AimRight(rate);
		}
		else if (rate < -0.4f || rate > 0.4f)
		{
			CameraLookRight(rate * 0.8f);
		}
	}
	else if (rate == 0 && CameraBoom->RelativeRotation.Yaw != 0 && !bBrangCameraYaw && !bCameraZoomIn)
	{
		bBrangCameraYaw = true;
	}
}

void AMainCharacter::AimRight(float rate)
{
	UMainCharacterAnimInstance * Animation = GetAnimationInstance();

	const FRotator YawRotation(0, rate, 0);

	bool bAddRotation = (rate > 0 && Animation->SkelControl_UpperArmstRotation.Yaw < AIM_BOUNDS_YAW_VALUE) || (rate < 0 && Animation->SkelControl_UpperArmstRotation.Yaw > -AIM_BOUNDS_YAW_VALUE);

	if (bAddRotation)
	{
		Animation->SkelControl_UpperArmstRotation += (YawRotation * BASE_AIM_AROUND_RATE * GetWorld()->GetDeltaSeconds());
	}
	else
	{
		AddYawRotation(rate * 0.5f, true);
	}
}

void AMainCharacter::CameraLookRight(float rate)
{
	bBrangCameraYaw = false;
	const FRotator YawRotation(0, rate, 0);
	const FRotator BoomRotation = CameraBoom->RelativeRotation;

	//adding Pitch and Yaw at the same time will result in Yaw add up
	bool bRollBoundsMet = BoomRotation.Roll > -ROTATION_BOUNDS_BASE_VALUE && BoomRotation.Roll < ROTATION_BOUNDS_BASE_VALUE;

	bool bAddRotation = (rate > 0 && BoomRotation.Yaw < ROTATION_BOUNDS_BASE_VALUE) || (rate < 0 && BoomRotation.Yaw > -ROTATION_BOUNDS_BASE_VALUE);

	if (bAddRotation && bRollBoundsMet)
	{
		CameraBoom->AddLocalRotation(YawRotation * BASE_LOOK_AROUND_RATE * GetWorld()->GetDeltaSeconds());
	}
}

void AMainCharacter::LookUp(float rate)
{
	if (rate != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		//in this case we are aiming
		if (bCameraZoomIn)
		{
			AimUp(rate);
		}
		else if (rate < -0.5f || rate > 0.5f)
		{
			CameraLookUp(rate);
		}
	}
	else if (rate == 0 && CameraBoom->RelativeRotation.Pitch != 0 && !bBrangCameraPitch && !bCameraZoomIn)
	{
		bBrangCameraPitch = true;
	}
}

void AMainCharacter::AimUp(float rate)
{
	if (!GetMesh()) return;

	UMainCharacterAnimInstance * Animation = Cast<UMainCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	//No Anim Instance Acquired?
	if (!Animation) return;

	const FRotator PitchRotation(rate, 0, 0);
	bool bAddRotation = (rate > 0 && Animation->SkelControl_UpperArmstRotation.Pitch < AIM_BOUNDS_PITCH_VALUE) || (rate < 0 && Animation->SkelControl_UpperArmstRotation.Pitch > -AIM_BOUNDS_PITCH_VALUE);

	if (bAddRotation)
	{
		Animation->SkelControl_UpperArmstRotation += (PitchRotation * BASE_AIM_AROUND_RATE * GetWorld()->GetDeltaSeconds());
		
		//start moving the camera as well
		if (Animation->SkelControl_UpperArmstRotation.Pitch > 20.f || Animation->SkelControl_UpperArmstRotation.Pitch < -20.f)
		{
			CameraBoom->AddLocalRotation(PitchRotation * BASE_AIM_AROUND_RATE * GetWorld()->GetDeltaSeconds());
		}
	}
}

void AMainCharacter::CameraLookUp(float rate)
{
	bBrangCameraPitch = false;
	const FRotator PitchRotation(rate, 0, 0);
	const FRotator BoomRotation = CameraBoom->RelativeRotation;

	//adding Pitch and Yaw at the same time will result in Roll add up
	bool bRollBoundsMet = BoomRotation.Roll > -ROTATION_BOUNDS_BASE_VALUE && BoomRotation.Roll < ROTATION_BOUNDS_BASE_VALUE;

	bool bAddRotation = (rate > 0 && BoomRotation.Pitch < ROTATION_BOUNDS_BASE_VALUE) || (rate < 0 && BoomRotation.Pitch > -ROTATION_BOUNDS_BASE_VALUE);

	if (bAddRotation && bRollBoundsMet)
	{
		CameraBoom->AddLocalRotation(PitchRotation * BASE_LOOK_AROUND_RATE * GetWorld()->GetDeltaSeconds());
	}
}

void AMainCharacter::BRangCameraPitch()
{
	const FRotator PitchRotation(CameraBoom->RelativeRotation.Pitch > INITIAL_ROTATION.Pitch ? -1 : 1, 0, 0);
	CameraBoom->AddLocalRotation(PitchRotation * BASE_LOOK_AROUND_RATE * GetWorld()->GetDeltaSeconds());

	bool bEndBrangPitch = (CameraBoom->RelativeRotation.Pitch >= 0 && CameraBoom->RelativeRotation.Pitch <= CAMERA_BRANG_CLAMP_RATE) || (CameraBoom->RelativeRotation.Pitch <= 0 && CameraBoom->RelativeRotation.Pitch >= -CAMERA_BRANG_CLAMP_RATE);

	if (bEndBrangPitch)
	{
		const FRotator InitialPitchRotation = FRotator(0, CameraBoom->RelativeRotation.Yaw, 0);
		CameraBoom->SetRelativeRotation(InitialPitchRotation);
		bBrangCameraPitch = false;
	}
}

void AMainCharacter::BRangCameraYaw()
{
	const FRotator YawRotation(0, CameraBoom->RelativeRotation.Yaw > INITIAL_ROTATION.Yaw ? -1 : 1, 0);
	CameraBoom->AddLocalRotation(YawRotation * BASE_LOOK_AROUND_RATE * GetWorld()->GetDeltaSeconds());

	bool bEndBrangYaw = (CameraBoom->RelativeRotation.Yaw >= 0 && CameraBoom->RelativeRotation.Yaw <= CAMERA_BRANG_CLAMP_RATE) || (CameraBoom->RelativeRotation.Yaw <= 0 && CameraBoom->RelativeRotation.Yaw >= -CAMERA_BRANG_CLAMP_RATE);

	if (bEndBrangYaw)
	{
		const FRotator InitialYawRotation = FRotator(CameraBoom->RelativeRotation.Pitch, 0, 0);
		CameraBoom->SetRelativeRotation(InitialYawRotation);
		bBrangCameraYaw = false;
	}
}

void AMainCharacter::BRangCharacterYaw()
{
	if (Controller && Controller->IsLocalPlayerController())
	{
		const FRotator YawRotation(0, FinalRotation_Character_Rotation.Yaw > INITIAL_ROTATION.Yaw ? 1 : -1, 0);
		APlayerController* const PC = CastChecked<APlayerController>(Controller);

		if ((PC->RotationInput.Yaw <= CAMERA_BRANG_CLAMP_RATE && PC->RotationInput.Yaw > 0.0f) || (PC->RotationInput.Yaw >= -CAMERA_BRANG_CLAMP_RATE && PC->RotationInput.Yaw < 0.0f))
		{
			PC->AddYawInput(YawRotation.Yaw);
		}
		else
		{
			bBrangCharacterYaw = false;
			PC->RotationInput.Yaw = FinalRotation_Character_Rotation.Yaw;
		}
	}
}

/** 
The animBP will take this var and run the animation depending on true or false */
void AMainCharacter::Equip_Pistol()
{
	//toggle this value
	bUnEquipPistol = bEquipPistol;
	bEquipPistol = !bEquipPistol;
}

/**
create a timeline and do the animation here, maybe create a timeline helper class for ths shit
*/
void AMainCharacter::Trigger_Aim_In()
{
	if (ActiveWeapon && GetSpawndedM9()->bIsWeaponAttached)
	{
		bCameraZoomIn = true;
		bCameraZoomOut = false;

		if (!GetMesh()) return;
		UMainCharacterAnimInstance * Animation = Cast<UMainCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		if (!Animation) return;

		Animation->ResetArmsRotation();

		bBrangCameraYaw = true;
		bBrangCameraPitch = true;
		
		//since we are facing a different direction we must turn our player
		if (CameraBoom->RelativeRotation.Yaw != 0)
		{
			bBrangCharacterYaw = true;
			FinalRotation_Character_Rotation = FRotator(0, CameraBoom->RelativeRotation.Yaw, 0);
		}

		/*
		//Todo nullbot: find out how to do this with the timeline helper
		AActor* myActor = Cast<AActor>(this);
		FName functionName = "Aim_ZoomIn";
		TimelineHelper* TLimeHelper = new TimelineHelper(myActor, functionName);
		*/
		//BRangZoomIn = false; sit down to create the aim animation
		//CameraBoom->TargetArmLength = 60.0f;
	}
}

void AMainCharacter::Trigger_Aim_Out()
{
	if (bEquipPistol)
	{
		bCameraZoomIn = false;
		bCameraZoomOut = true;
	}
	
}

void AMainCharacter::Aim_In()
{
	if (CameraBoom->TargetArmLength > BASE_CAMERA_ZOOMED_ARM_LEGNTH)
	{
		CameraBoom->TargetArmLength -= ZOOM_RATE;
	}
}

void AMainCharacter::Aim_Out()
{
	if (CameraBoom->TargetArmLength < BASE_CAMERA_ARM_LEGNTH)
	{
		CameraBoom->TargetArmLength += ZOOM_RATE;
	}
	else
	{
		bCameraZoomOut = false;
	}
}

void AMainCharacter::StartReloading()
{
	//only reload when you are aiming
	if (ActiveWeapon && bCameraZoomIn && AmmoAvailable > 0)
	{
		//goes back to false on the animation blueprint
		bReloadPistol = true;
	}
}

void AMainCharacter::ReloadWeapon()
{
	//only reload when you are aiming
	if (ActiveWeapon)
	{
		AWeapon_M9* SpawnedWeapon = GetSpawndedM9();

		int32 AmmoMissing = SpawnedWeapon->AmmoCapacity - SpawnedWeapon->Ammo;
		int32 AmmoAfterReload = AmmoAvailable - AmmoMissing;
		int32 AmmoToReload = AmmoAfterReload < 0 ? AmmoAvailable : AmmoMissing;
		
		SpawnedWeapon->Reload(AmmoToReload);
		AmmoAvailable = AmmoAfterReload < 0 ? 0 : AmmoAfterReload;
	}
}

void AMainCharacter::ShootWeapon()
{
	if (ActiveWeapon && bCameraZoomIn && !bReloadPistol && GetSpawndedM9()->Ammo > 0)
	{
		GetSpawndedM9()->ShootWeapon();

		//Do Recoil on the player
		RecoilAnimation();
	}
	else
	{
		//TODO: 
	}
}

void AMainCharacter::RecoilAnimation()
{
	//recoil animation
	UMainCharacterAnimInstance * Animation = Cast<UMainCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	//No Anim Instance Acquired?
	if (!Animation) return;
	Animation->RecoilAnimation();
}

UMainCharacterAnimInstance* AMainCharacter::GetAnimationInstance()
{
	if (!GetMesh()) return new UMainCharacterAnimInstance();

	UMainCharacterAnimInstance * Animation = Cast<UMainCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	//No Anim Instance Acquired?
	if (!Animation) return new UMainCharacterAnimInstance();
	return Animation;
}

AWeapon_M9* AMainCharacter::GetSpawndedM9()
{
	AWeapon_M9* SpawnedWeapon = Cast<AWeapon_M9>(ActiveWeapon);
	return SpawnedWeapon;
}

//TODO nullBot: add a parameter, enum maybe for weapon types
void AMainCharacter::AttachOrDetachWeapon()
{
	//Check for a valid World
	UWorld * const World = GetWorld();
	if (World)
	{
		if (bEquipPistol)
		{
			ActiveWeapon = GetWorld()->SpawnActor<AWeapon_M9>(AWeapon_M9::StaticClass());
			GetSpawndedM9()->AddWeapon(GetMesh(), "hand_rPistolSocket");
		}
		else
		{
			GetSpawndedM9()->DestroyWeapon();
			//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Yellow, FString::FString("Weapon Destroyed"));
		}
		UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
		DataInstance->OnWeaponEquipped.Broadcast();
	}
}

void AMainCharacter::MakePistolEquipped()
{
	GetAnimationInstance()->bEquipPistolAnimation = true;
	bEquipPistol = true;
	GetAnimationInstance()->bIsPistolEquipped = true;
	AttachOrDetachWeapon();
}

void AMainCharacter::ExecuteContextAction()
{
	if (ActionListener)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0, FColor::Magenta, FString::FString("EXECUTE CONTEXT ACTION"));
		ActionListener->OnContextAction();
	}
}

void AMainCharacter::OnRegisterActorAsListener(AActor * IteractiveActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Character - Enter Overlap"));
	ActionListener = Cast<ABasicButton>(IteractiveActor);
}

void AMainCharacter::OnUnregisterActorAsListener()
{
	UE_LOG(LogTemp, Warning, TEXT("Character - Exit Overlap"));
	ActionListener = nullptr;
}

void AMainCharacter::PauseGame()
{

}
