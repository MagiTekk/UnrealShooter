// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "RotatableTarget.h"
#include "Engine/DestructibleMesh.h"

ARotatableTarget::ARotatableTarget()
{
	ARotatableTarget::InitTarget();
}

// Sets default values
ARotatableTarget::ARotatableTarget(ETargetType targetType) //TODO _nullBot: depending on the target type choose a material to dynamically instantiate
{
	TargetType = targetType;
	ARotatableTarget::InitTarget();
}

void ARotatableTarget::InitTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	this->SetRootComponent(DefaultSceneRoot);

	ConstructorHelpers::FObjectFinder<UDestructibleMesh> meshHead(TEXT("DestructibleMesh'/Game/UnrealShooter/Mesh/Target/TargetMesh_Cube_DM.TargetMesh_Cube_DM'"));
	HeadMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("HeadMesh"));
	HeadMesh->SetDestructibleMesh(meshHead.Object);
	HeadMesh->SetMobility(EComponentMobility::Movable);
	HeadMesh->AttachTo(DefaultSceneRoot);

	ConstructorHelpers::FObjectFinder<UStaticMesh> meshBody(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetStaticMesh(meshBody.Object);
	BodyMesh->AttachTo(DefaultSceneRoot);

	ConstructorHelpers::FObjectFinder<UStaticMesh> meshBase(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetStaticMesh(meshBase.Object);
	BaseMesh->AttachTo(DefaultSceneRoot);

	ARotatableTarget::InitMaterialInstance();

	//listen for my destructible's onFracture signal
	FScriptDelegate OnHeadFractured;
	OnHeadFractured.BindUFunction(this, "OnHeadFractured");
	HeadMesh->OnComponentFracture.AddUnique(OnHeadFractured);

	//init bool vars
	bHeadDestroyed = false;
	bRaiseTarget = false;
	bLowerTarget = false;
	bVanish = false;
}

//load the desired material instance
void ARotatableTarget::InitMaterialInstance()
{
	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> LowMaterialObj(TEXT("MaterialInstanceConstant'/Game/UnrealShooter/Materials/Instances/Targets/BasicMaterial_Inst_LowTarget.BasicMaterial_Inst_LowTarget'"));
	lowtMaterialInst = LowMaterialObj.Object;
	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> MidMaterialObj(TEXT("MaterialInstanceConstant'/Game/UnrealShooter/Materials/Instances/Targets/BasicMaterial_Inst_MidTarget.BasicMaterial_Inst_MidTarget'"));
	midMaterialInst = MidMaterialObj.Object;
	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> FalseMaterialObj(TEXT("MaterialInstanceConstant'/Game/UnrealShooter/Materials/Instances/Targets/BasicMaterial_Inst_FalseTarget.BasicMaterial_Inst_FalseTarget'"));
	falseMaterialInst = FalseMaterialObj.Object;
	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> DefaultMaterialObj(TEXT("MaterialInstanceConstant'/Game/UnrealShooter/Materials/Instances/Targets/BasicMaterial_Inst_DefaultTarget.BasicMaterial_Inst_DefaultTarget'"));
	defaultMaterialInst = DefaultMaterialObj.Object;
}

// Called when the game starts or when spawned
void ARotatableTarget::BeginPlay()
{
	Super::BeginPlay();

	//start on the ground
	ARotatableTarget::RaiseTarget();

	//TEST, delete
	//GetWorld()->GetTimerManager().SetTimer(TargetTimerHandle, this, &ARotatableTarget::RaiseTarget, 1.0f, false);
}

void ARotatableTarget::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//you could also create the dynamic instance using the material and not the instance.
	//DynamicInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);

	//create dynamic instance and apply it to all the meshes
	DynamicInstance = UMaterialInstanceDynamic::Create(ARotatableTarget::GetMaterialInstance(), this);
	HeadMesh->SetMaterial(0, DynamicInstance);
	HeadMesh->SetMaterial(1, DynamicInstance);
	BodyMesh->SetMaterial(0, DynamicInstance);
	BaseMesh->SetMaterial(0, DynamicInstance);

	//DynamicInstance->SetScalarParameterValue("Multi", 1);
	//DynamicInstance->SetScalarParameterValue("OpacityModifier", 1);
}

UMaterialInstanceConstant* ARotatableTarget::GetMaterialInstance()
{
	switch (TargetType)
	{
		case ETargetType::LowTarget:
			return lowtMaterialInst;
			break;
		case ETargetType::MidTarget:
			return midMaterialInst;
			break;
		case ETargetType::FalseTarget:
			return falseMaterialInst;
			break;
		case ETargetType::DefaultTarget:
		default:
			return defaultMaterialInst;
			break;
	}
}

// Called every frame
void ARotatableTarget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bRaiseTarget)
	{
		ARotatableTarget::DoTargetUp();
	}
	if (bLowerTarget)
	{
		ARotatableTarget::DoTargetDown();
	}
	if (bVanish)
	{
		ARotatableTarget::Vanish();
	}

}

//activates the raise target animation
void ARotatableTarget::RaiseTarget()
{
	bRaiseTarget = true;
}

//activates the lower target animation
void ARotatableTarget::LowerTarget()
{
	bLowerTarget = true;
}

//Tick's function raise target until desired angle
void ARotatableTarget::DoTargetUp()
{
	FRotator actorRotation = GetActorRotation();
	if (actorRotation.Pitch > RAISED_ROTATION)
	{
		FRotator newRotation = FRotator{ actorRotation.Pitch - ROTATIONAL_RATE, GetActorRotation().Yaw, GetActorRotation().Roll };
		SetActorRotation(newRotation);
	}
	else
	{
		bRaiseTarget = false;
		GetWorld()->GetTimerManager().SetTimer(TargetTimerHandle, this, &ARotatableTarget::LowerTarget, 5.0f, false);
	}
}

void ARotatableTarget::DoTargetDown()
{
	FRotator actorRotation = GetActorRotation();
	if (actorRotation.Pitch < LOWERED_ROTATION)
	{
		FRotator newRotation = FRotator{ actorRotation.Pitch + ROTATIONAL_RATE, GetActorRotation().Yaw, GetActorRotation().Roll };
		SetActorRotation(newRotation);
	}
	else
	{
		bLowerTarget = false;
		if (!bHeadDestroyed)
		{
			ARotatableTarget::Die();
		}
	}
}

void ARotatableTarget::OnHeadFractured(const FVector& HitPoint, const FVector& HitDirection)
{
	bHeadDestroyed = true;
	ARotatableTarget::startVanish();
}

void ARotatableTarget::OnTargetHit()
{
	ARotatableTarget::LowerTarget();
}

void ARotatableTarget::startVanish()
{
	//start vanishing actor
	bVanish = true;
}

//vanish this actor
void ARotatableTarget::Vanish()
{
	float opacityVal;
	DynamicInstance->GetScalarParameterValue("OpacityModifier", opacityVal);
	
	if (opacityVal > 0.0f)
	{
		DynamicInstance->SetScalarParameterValue("OpacityModifier", opacityVal - 0.005f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), "BYE BYE");
		bVanish = false;
		ARotatableTarget::Die();
	}
}

void ARotatableTarget::Die()
{
	//clear timer
	GetWorld()->GetTimerManager().ClearTimer(TargetTimerHandle);

	//destroy
	Destroy();
}
