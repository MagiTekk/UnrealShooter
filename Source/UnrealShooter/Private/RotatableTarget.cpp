// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "BasicSpawnPoint.h"
#include "RotatableTarget.h"
#include "ExplosiveActor.h"
#include "UnrealShooterLevelScriptActor.h"
#include "UnrealShooterDataSingleton.h"
#include "Engine/DestructibleMesh.h"

ARotatableTarget::ARotatableTarget()
{
	ARotatableTarget::InitTarget();
}

void ARotatableTarget::InitTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	this->SetRootComponent(DefaultSceneRoot);

	HeadCenterPointScene = CreateDefaultSubobject<USceneComponent>(TEXT("HeadCenterPointScene"));
	HeadCenterPointScene->AttachTo(DefaultSceneRoot);

	//I had to simulate physics and disable gravity because of a bug where the DM is not visible after being spawned by code
	ConstructorHelpers::FObjectFinder<UDestructibleMesh> destructibleCube(TEXT("DestructibleMesh'/Game/UnrealShooter/Mesh/Target/TargetMesh_Cube_DM.TargetMesh_Cube_DM'"));
	HeadMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("HeadMesh"));
	HeadMesh->SetDestructibleMesh(destructibleCube.Object);
	HeadMesh->SetMobility(EComponentMobility::Movable);
	HeadMesh->AttachTo(DefaultSceneRoot);

	BodyMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("BodyMesh"));
	BodyMesh->SetDestructibleMesh(destructibleCube.Object);
	BodyMesh->SetMobility(EComponentMobility::Movable);
	BodyMesh->AttachTo(DefaultSceneRoot);

	ConstructorHelpers::FObjectFinder<UStaticMesh> meshBase(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetStaticMesh(meshBase.Object);
	BaseMesh->AttachTo(DefaultSceneRoot);

	Dynamite = CreateDefaultSubobject<UChildActorComponent>(TEXT("Dynamite"));
	Dynamite->SetChildActorClass(AExplosiveActor::StaticClass());
	Dynamite->AttachTo(DefaultSceneRoot);

	PointsTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PointsTextRender"));
	//PointsTextRender->SetVisibility(false);
	PointsTextRender->AttachTo(DefaultSceneRoot);

	InitMaterialInstance();

	//listen for my destructible's onFracture signal
	FScriptDelegate OnHeadFractured;
	OnHeadFractured.BindUFunction(this, "OnHeadFractured");
	HeadMesh->OnComponentFracture.AddUnique(OnHeadFractured);

	RotationalRate = DEFAULT_ROTATIONAL_RATE;
}

//load the desired material instance
void ARotatableTarget::InitMaterialInstance()
{
	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> DefaultMaterialObj(TEXT("MaterialInstanceConstant'/Game/UnrealShooter/Materials/Instances/M_IcyTarget_Inst.M_IcyTarget_Inst'"));
	DefaultMaterialInst = DefaultMaterialObj.Object;
	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> TransparentMaterialObj(TEXT("MaterialInstanceConstant'/Game/UnrealShooter/Materials/Instances/BasicTransparentMaterial_Inst.BasicTransparentMaterial_Inst'"));
	TransparentMaterialInst = TransparentMaterialObj.Object;
}

// Called when the game starts or when spawned
void ARotatableTarget::BeginPlay()
{
	Super::BeginPlay();
}

void ARotatableTarget::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//create dynamic instance and apply it to all the meshes
	UpdateMaterialInstance();
}

void ARotatableTarget::UpdateMaterialInstance()
{
	if (!bIsFrozen)
	{
		DynamicInstance = UMaterialInstanceDynamic::Create(bIsTranslucent ? TransparentMaterialInst : DefaultMaterialInst, this);
		DynamicInstance->SetVectorParameterValue("BaseColor", ARotatableTarget::GetMaterialColor());
		HeadMesh->SetMaterial(0, DynamicInstance);
		HeadMesh->SetMaterial(1, DynamicInstance);
		BodyMesh->SetMaterial(0, DynamicInstance);
		BodyMesh->SetMaterial(1, DynamicInstance);
		BaseMesh->SetMaterial(0, DynamicInstance);
	}
}

FLinearColor ARotatableTarget::GetMaterialColor()
{
	switch (TargetProperties.TargetType)
	{
		case ETargetType::MaleTarget:
			return LOWTARGET_COLOR;
			break;
		case ETargetType::FemaleTarget:
			return MIDTARGET_COLOR;
			break;
		case ETargetType::InnocentTarget:
			return FALSETARGET_COLOR;
			break;
		default:
			return DEFAULTTARGET_COLOR;
			break;
	}
}

void ARotatableTarget::ApplyProperties(FRotatableTargetProperties TargetProperties, int32 TimeToLive)
{
	this->TargetProperties = TargetProperties;
	this->SetActorLocation(GetSpawnPoint(TargetProperties.InitialLocation));
	this->TimeToLive = TimeToLive;

	//by default all targets spawn on the ground
	this->SetActorRotation(FRotator{ LOWERED_ROTATION, 0.0f, 0.0f });

	//comment out this call and you will have a default explosive on every target!
	InitDynamite();

	SetNewLocation();
	UpdateMaterialInstance();
	RaiseTarget();
}

void ARotatableTarget::InitDynamite()
{
	if (TargetProperties.ExplosiveType == EExplosiveType::NonExplosive)
	{
		Dynamite->DestroyComponent();
	}
	else
	{
		AExplosiveActor* DynamiteActor = Cast<AExplosiveActor>(Dynamite->ChildActor);
		if (DynamiteActor)
		{
			DynamiteActor->ApplyProperties(TargetProperties.ExplosiveType);
		}
	}
}

// Called every frame
void ARotatableTarget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bRaiseTarget)
	{
		DoTargetUp();
	}
	if (bLowerTarget)
	{
		DoTargetDown();
	}
	if (bVanish)
	{
		Vanish();
	}
	if (bMoveTarget)
	{
		UpdateTargetLocation();
	}
}

//activates the raise target animation
void ARotatableTarget::RaiseTarget()
{
	bRaiseTarget = true;
	bLowerTarget = false;
}

//activates the lower target animation
void ARotatableTarget::LowerTarget()
{
	bRaiseTarget = false;
	bLowerTarget = true;
}

//Tick's function raise target until desired angle
void ARotatableTarget::DoTargetUp()
{
	FRotator actorRotation = GetActorRotation();
	if (actorRotation.Pitch < RAISED_ROTATION)
	{
		FRotator newRotation = FRotator{ actorRotation.Pitch + RotationalRate, actorRotation.Yaw, actorRotation.Roll };
		SetActorRotation(newRotation);
	}
	else
	{
		bRaiseTarget = false;
	}
}

void ARotatableTarget::DoTargetDown()
{
	FRotator actorRotation = GetActorRotation();
	if (actorRotation.Pitch > LOWERED_ROTATION)
	{
		FRotator newRotation = FRotator{ actorRotation.Pitch - RotationalRate, actorRotation.Yaw, actorRotation.Roll };
		SetActorRotation(newRotation);
	}
	else
	{
		bLowerTarget = false;
		Die();
	}
}

FVector ARotatableTarget::GetSpawnPoint(FVector SpawnPosition)
{
	for (TActorIterator<ABasicSpawnPoint> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->SpawnPosition == SpawnPosition)
		{
			return ActorItr->GetActorLocation();
		}
	}
	return FVector();
}

//if we hold a location on our properties we will move there at a certain speed, after that the target goes down
void ARotatableTarget::SetNewLocation()
{
	if (TargetProperties.Locations.Num() > 0)
	{
		FVector location = TargetProperties.Locations[0];
		NextLocation = GetSpawnPoint(location);
		TargetProperties.Locations.Remove(location);
		bMoveTarget = true;
	}
	else
	{
		//no more locations to go? then lower our target
		GetWorld()->GetTimerManager().SetTimer(TargetTimerHandle, this, &ARotatableTarget::LowerTarget, this->TimeToLive, false);
	}
}

void ARotatableTarget::UpdateTargetLocation()
{
	//shall we move on X or Y?
	FVector actorLocation = this->GetActorLocation();
	bool bMoveX = actorLocation.Y == NextLocation.Y;
	bool bMoveY = actorLocation.X == NextLocation.X;

	//in which direction are we moving?
	float xMovementRate = actorLocation.X <= NextLocation.X ? TargetProperties.Speed : TargetProperties.Speed * -1;
	float yMovementRate = actorLocation.Y <= NextLocation.Y ? TargetProperties.Speed : TargetProperties.Speed * -1;

	//we must check if we arrived at our destination
	if (bMoveX && xMovementRate > 0)
	{
		if (actorLocation.X >= NextLocation.X)
		{
			bMoveTarget = false;
		}
	}
	else if (bMoveX && xMovementRate < 0)
	{
		if (actorLocation.X <= NextLocation.X)
		{
			bMoveTarget = false;
		}
	}
	else if (bMoveY && yMovementRate > 0)
	{
		if (actorLocation.Y >= NextLocation.Y)
		{
			bMoveTarget = false;
		}
	}
	else
	{
		if (actorLocation.Y <= NextLocation.Y)
		{
			bMoveTarget = false;
		}
	}

	if (bMoveTarget)
	{
		FVector newLocation = { bMoveX ? actorLocation.X + xMovementRate : actorLocation.X,
								bMoveY ? actorLocation.Y + yMovementRate : actorLocation.Y,
								actorLocation.Z };

		this->SetActorLocation(newLocation);
	}
	else
	{
		//just o be sure, set the location to exactly the target's location
		this->SetActorLocation(NextLocation);

		//get a new location and continue moving on
		SetNewLocation();
	}
}

//calback fired whenever the destructible mesh is destroyed, unused
void ARotatableTarget::OnHeadFractured(const FVector& HitPoint, const FVector& HitDirection)
{
	bIsHeadShot = true;
}

void ARotatableTarget::OnTargetHit(bool forceHit)
{
	//in case the target has been hit already
	if (!bIsTranslucent && !bIsElectrified || forceHit)
	{
		if (!bIsFrozen)
		{
			bIsTranslucent = true;
		}
		UpdateMaterialInstance();
		LowerTarget();
		startVanish();
		RewardTargetPoints();

		//chain hits for special target display
		AUnrealShooterLevelScriptActor* MyLvlBP = Cast<AUnrealShooterLevelScriptActor>(GetWorld()->GetLevelScriptActor());
		if (MyLvlBP)
		{
			MyLvlBP->OnTargetHit();
		}
	}
}

void ARotatableTarget::startVanish()
{
	//start vanishing actor
	bVanish = true;
}

void ARotatableTarget::Freeze()
{
	bIsFrozen = true;
	DynamicInstance->SetScalarParameterValue("IceBlend", 0.4f);
	this->SetActorTickEnabled(false);
	GetWorld()->GetTimerManager().SetTimer(FreezeTimerHandle, this, &ARotatableTarget::UnFreeze, FROZEN_TIME, false);
}

void ARotatableTarget::UnFreeze()
{
	bIsFrozen = false;
	UpdateMaterialInstance();
	DynamicInstance->SetScalarParameterValue("IceBlend", 0.0f);
	this->SetActorTickEnabled(true);
}

void ARotatableTarget::LightningIncoming()
{
	bIsElectrified = true;
	GetWorld()->GetTimerManager().SetTimer(LightningTimerHandle, this, &ARotatableTarget::LightningStrike, LiGHTNING_TIME, false);
}

void ARotatableTarget::LightningStrike()
{
	HeadMesh->ApplyRadiusDamage(100.0f, HeadCenterPointScene->GetComponentLocation(), 360.0f, 100.0f, true);

	//timer with parameters, they have to be passed using a FTimerDelegate
	FTimerDelegate LightningDelegate = FTimerDelegate::CreateUObject(this, &ARotatableTarget::OnTargetHit, true);
	GetWorld()->GetTimerManager().SetTimer(LightningTimerHandle, LightningDelegate, 1.0f, false);
}

//vanish this actor
void ARotatableTarget::Vanish()
{
	float opacityVal;
	DynamicInstance->GetScalarParameterValue("OpacityModifier", opacityVal);
	
	if (opacityVal > 0.0f)
	{
		DynamicInstance->SetScalarParameterValue("OpacityModifier", opacityVal - 0.01f);
	}
	else
	{
		//target has been vanished!
		bVanish = false;
	}
}

void ARotatableTarget::RewardTargetPoints()
{
	AUnrealShooterLevelScriptActor* MyLvlBP = Cast<AUnrealShooterLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (MyLvlBP)
	{
		MyLvlBP->RewardTargetPoints(bIsHeadShot ? TargetProperties.HeadshotPoints : TargetProperties.Points);
		bIsHeadShot = false; //when target is frozen you can get double points if you don't do this
	}
}

void ARotatableTarget::Die()
{
	//clear timer
	GetWorld()->GetTimerManager().ClearTimer(TargetTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(FreezeTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(LightningTimerHandle);

	//launch a signal to update our sequence
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnTargetDestroyed.Broadcast();

	//get rid of the dynamite
	if (Dynamite)
	{
		Dynamite->DestroyComponent();
	}

	//destroy
	Destroy();
}
