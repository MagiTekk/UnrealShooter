// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "Explosion.h"
#include "BasicSpawnPoint.h"
#include "SpecialTarget.h"
#include "Engine/DestructibleMesh.h"
#include "UnrealShooterLevelScriptActor.h"
#include "UnrealShooterDataSingleton.h"


// Sets default values
ASpecialTarget::ASpecialTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	this->SetRootComponent(DefaultSceneRoot);

	//I had to simulate physics and disable gravity because of a bug where the DM is not visible after being spawned by code
	ConstructorHelpers::FObjectFinder<UDestructibleMesh> destructibleCube(TEXT("DestructibleMesh'/Game/UnrealShooter/Mesh/Target/TargetMesh_Cube_DM.TargetMesh_Cube_DM'"));
	DestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleMesh"));
	DestructibleMesh->SetDestructibleMesh(destructibleCube.Object);
	DestructibleMesh->SetMobility(EComponentMobility::Movable);
	DestructibleMesh->AttachTo(DefaultSceneRoot);

	//listen for my destructible's onFracture signal
	FScriptDelegate OnMeshFractured;
	OnMeshFractured.BindUFunction(this, "OnMeshFractured");
	DestructibleMesh->OnComponentFracture.AddUnique(OnMeshFractured);

	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> DefaultMaterialObj(TEXT("MaterialInstanceConstant'/Game/UnrealShooter/Materials/Instances/ExplosiveMatinstance.ExplosiveMatinstance'"));
	DefaultMaterialInst = DefaultMaterialObj.Object;

	//Explosion Actor
	ConstructorHelpers::FObjectFinder<UClass> explosion(TEXT("Blueprint'/Game/UnrealShooter/Blueprint/Effects/ExplosionBP.ExplosionBP_C'"));
	ExplosionBP = explosion.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> specialEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/Ice/Ice_effect.Ice_effect'"));
	SpecialParticleEffectReference = specialEmitter.Object;

	ExplosiveTypeParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosiveTypeParticleEffect"));
	ExplosiveTypeParticleEffect->SetTemplate(SpecialParticleEffectReference);
	ExplosiveTypeParticleEffect->AttachTo(RootComponent);
}

void ASpecialTarget::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//create dynamic instance and apply it to all the meshes
	UpdateMaterialInstance();
}

void ASpecialTarget::UpdateMaterialInstance()
{
	DynamicInstance = UMaterialInstanceDynamic::Create(DefaultMaterialInst, this);
	DynamicInstance->SetVectorParameterValue("BaseColor", DEFAULTTARGET_COLOR);
	DestructibleMesh->SetMaterial(0, DynamicInstance);
	DestructibleMesh->SetMaterial(1, DynamicInstance);
}

// Called when the game starts or when spawned
void ASpecialTarget::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpecialTarget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bMoveTarget)
	{
		UpdateTargetLocation();
	}
}

void ASpecialTarget::ApplyProperties(FSpecialTargetProperties props)
{
	this->TargetProperties = props;
	this->SetActorLocation(GetSpawnPoint(TargetProperties.InitialLocation));
	SetNewLocation();
}

void ASpecialTarget::OnMeshFractured(const FVector & HitPoint, const FVector & HitDirection)
{
	//create explosion and do everything you need to make this super cool
	ExplosiveTypeParticleEffect->DestroyComponent();

	//spawn a blast particle effect on the location this actor was when the mesh fracture happened
	AExplosion* explosionActor = GetWorld()->SpawnActor<AExplosion>(ExplosionBP, GetActorLocation(), FRotator(0, 0, 0));
	explosionActor->ApplyProperties(EExplosiveType::NonExplosive);
	explosionActor->Explode();

	//reward points && shake the camera effect
	AUnrealShooterLevelScriptActor* MyLvlBP = Cast<AUnrealShooterLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (MyLvlBP)
	{
		MyLvlBP->CameraShake();
		MyLvlBP->OnTargetHit(false, true);
		MyLvlBP->DisplayRewardedPoints(TargetProperties.Points, this->GetActorLocation());
	}

	Die();
}

FVector ASpecialTarget::GetSpawnPoint(FVector SpawnPosition)
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
void ASpecialTarget::SetNewLocation()
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
		Die();
	}
}

void ASpecialTarget::UpdateTargetLocation()
{
	//shall we move on X or Y?
	FVector actorLocation = this->GetActorLocation();
	bool bMoveX = actorLocation.Y == NextLocation.Y;
	bool bMoveY = actorLocation.X == NextLocation.X;

	//in which direction are we moving?
	float xMovementRate = actorLocation.X <= NextLocation.X ? TargetProperties.Speed : TargetProperties.Speed * -1;
	float yMovementRate = actorLocation.Y <= NextLocation.Y ? TargetProperties.Speed : TargetProperties.Speed * -1;

	//check out the new location
	FVector newLocation = { bMoveX ? actorLocation.X + xMovementRate : actorLocation.X,
		bMoveY ? actorLocation.Y + yMovementRate : actorLocation.Y,
		actorLocation.Z };

	//will the new location place the actor at or after our destination?
	if (bMoveX && xMovementRate > 0)
	{
		if (newLocation.X >= NextLocation.X)
		{
			bMoveTarget = false;
		}
	}
	else if (bMoveX && xMovementRate < 0)
	{
		if (newLocation.X <= NextLocation.X)
		{
			bMoveTarget = false;
		}
	}
	else if (bMoveY && yMovementRate > 0)
	{
		if (newLocation.Y >= NextLocation.Y)
		{
			bMoveTarget = false;
		}
	}
	else
	{
		if (newLocation.Y <= NextLocation.Y)
		{
			bMoveTarget = false;
		}
	}

	if (bMoveTarget)
	{
		this->SetActorLocation(newLocation);
	}
	else
	{
		//just to be sure, set the location to exactly the target's location
		this->SetActorLocation(NextLocation);

		//get a new location and continue moving on
		SetNewLocation();
	}
}

void ASpecialTarget::Die()
{
	//launch a signal to update our sequence
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnTargetDestroyed.Broadcast(this);

	Destroy();
}

