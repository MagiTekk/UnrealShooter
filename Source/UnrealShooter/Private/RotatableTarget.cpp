// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "RotatableTarget.h"
#include "Engine/DestructibleMesh.h"


// Sets default values
ARotatableTarget::ARotatableTarget()
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
	BodyMesh->AttachTo(HeadMesh);

	ConstructorHelpers::FObjectFinder<UMaterial> MatObj(TEXT("Material'/Game/UnrealShooter/Materials/BasicMaterial.BasicMaterial'"));
	if (MatObj.Succeeded()) BaseMaterial = MatObj.Object;
	
	//init bool vars
	bRaiseTarget = false;
	bLowerTarget = false;
	bVanish = false;
}

// Called when the game starts or when spawned
void ARotatableTarget::BeginPlay()
{
	Super::BeginPlay();

	//TEST, delete
	/* Activate the fuze to explode the bomb after several seconds */
	GetWorld()->GetTimerManager().SetTimer(TargetTimerHandle, this, &ARotatableTarget::RaiseTarget, 5.0f, false);
}

void ARotatableTarget::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (BaseMaterial)
	{
		DynamicInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		HeadMesh->SetMaterial(0, DynamicInstance);
		HeadMesh->SetMaterial(1, DynamicInstance);
		BodyMesh->SetMaterial(0, DynamicInstance);
		DynamicInstance->SetScalarParameterValue("Multi", 1);
		//DynamicInstance->SetScalarParameterValue("OpacityModifier", 1);
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
	GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, FString::FString("RAISING TARGET NOW"));
}

//activates the lower target animation
void ARotatableTarget::LowerTarget()
{
	bLowerTarget = true;
	GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, FString::FString("LOWERING TARGET NOW"));
}

//Tick's function raise target until desired angle
void ARotatableTarget::DoTargetUp()
{
	FRotator actorRotation = GetActorRotation();
	if (actorRotation.Pitch < RAISED_ROTATION)
	{
		FRotator newRotation = FRotator{ actorRotation.Pitch + ROTATIONAL_RATE, GetActorRotation().Yaw, GetActorRotation().Roll };
		SetActorRotation(newRotation);
	}
	else
	{
		bRaiseTarget = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Magenta, FString::FString("TARGET RAISED, DO DESTROY"));
		ARotatableTarget::Kill();
		//GetWorld()->GetTimerManager().SetTimer(TargetTimerHandle, this, &ARotatableTarget::LowerTarget, 5.0f, false);
	}
}

void ARotatableTarget::DoTargetDown()
{
	FRotator actorRotation = GetActorRotation();
	if (actorRotation.Pitch > LOWERED_ROTATION)
	{
		FRotator newRotation = FRotator{ actorRotation.Pitch - ROTATIONAL_RATE, GetActorRotation().Yaw, GetActorRotation().Roll };
		SetActorRotation(newRotation);
	}
	else
	{
		bLowerTarget = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Magenta, FString::FString("TARGET LOWERED, DO DESTROY"));
		ARotatableTarget::Kill();
	}
}

void ARotatableTarget::Kill()
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
