// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "ExplosiveActor.h"
#include "Explosion.h"
#include "Engine/DestructibleMesh.h"
#include "UnrealShooterLevelScriptActor.h"


AExplosiveActor::AExplosiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	this->SetRootComponent(DefaultSceneRoot);

	ConstructorHelpers::FObjectFinder<UDestructibleMesh> dynamiteMeshBP(TEXT("DestructibleMesh'/Game/UnrealShooter/Mesh/Target/TargetMesh_Cube_DM.TargetMesh_Cube_DM'"));
	DynamiteMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DynamiteMesh"));
	DynamiteMesh->SetDestructibleMesh(dynamiteMeshBP.Object);
	DynamiteMesh->SetMobility(EComponentMobility::Movable);
	DynamiteMesh->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepRelativeTransform);

	//Igniter particle effect
	ConstructorHelpers::FObjectFinder<UParticleSystem> igniterEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/IgniterEffect.IgniterEffect'"));
	IgniterParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IgniterParticleEffect"));
	IgniterParticleEffect->SetTemplate(igniterEmitter.Object);
	IgniterParticleEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Particle Effects
	ConstructorHelpers::FObjectFinder<UParticleSystem> fireEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/Fire/P_Fire_Torch_01.P_Fire_Torch_01'"));
	FireParticleEffectReference = fireEmitter.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> iceEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/Ice/Ice_effect.Ice_effect'"));
	IceParticleEffectReference = iceEmitter.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> lightningEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/Lightning/P_DOT_Lightning_01.P_DOT_Lightning_01'"));
	LightningParticleEffectReference = lightningEmitter.Object;

	ExplosiveTypeParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosiveTypeParticleEffect"));
	ExplosiveTypeParticleEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Explosion Actor
	ConstructorHelpers::FObjectFinder<UClass> explosion(TEXT("Blueprint'/Game/UnrealShooter/Blueprint/Effects/ExplosionBP.ExplosionBP_C'"));
	ExplosionBP = explosion.Object;

	//Material
	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> FiretMaterialReference(TEXT("MaterialInstanceConstant'/Game/UnrealShooter/Materials/Instances/M_JaggedRockFire_MeshEmit_Lit_Inst.M_JaggedRockFire_MeshEmit_Lit_Inst'"));
	FireMaterialInst = FiretMaterialReference.Object;

	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> IcetMaterialReference(TEXT("MaterialInstanceConstant'/Game/UnrealShooter/Materials/Instances/M_JaggedRockIce_MeshEmit_Inst.M_JaggedRockIce_MeshEmit_Inst'"));
	IceMaterialInst = IcetMaterialReference.Object;

	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> LightningtMaterialReference(TEXT("MaterialInstanceConstant'/Game/UnrealShooter/Materials/Instances/M_JaggedRockLightning_MeshEmit_Lit_Inst.M_JaggedRockLightning_MeshEmit_Lit_Inst'"));
	LightningMaterialInst = LightningtMaterialReference.Object;

	//listen for my destructible's onFracture signal
	FScriptDelegate OnDynamiteFractured;
	OnDynamiteFractured.BindUFunction(this, "OnDynamiteFractured");
	DynamiteMesh->OnComponentFracture.AddUnique(OnDynamiteFractured);
}

// Called when the game starts or when spawned
void AExplosiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AExplosiveActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//create dynamic instance and apply it
	ApplyProperties(explosiveType);
}

void AExplosiveActor::ApplyProperties(EExplosiveType type)
{
	explosiveType = type;
	switch (explosiveType)
	{
		case EExplosiveType::Fire:
			DynamicInstance = UMaterialInstanceDynamic::Create(FireMaterialInst, this);
			ExplosiveTypeParticleEffect->SetTemplate(FireParticleEffectReference);
			break;
		case EExplosiveType::Ice:
			DynamicInstance = UMaterialInstanceDynamic::Create(IceMaterialInst, this);
			ExplosiveTypeParticleEffect->SetTemplate(IceParticleEffectReference);
			break;
		case EExplosiveType::Lightning:
			DynamicInstance = UMaterialInstanceDynamic::Create(LightningMaterialInst, this);
			ExplosiveTypeParticleEffect->SetTemplate(LightningParticleEffectReference);
			break;
		default:
			DynamicInstance = UMaterialInstanceDynamic::Create(FireMaterialInst, this);
			ExplosiveTypeParticleEffect->SetTemplate(FireParticleEffectReference);
			break;
	}
	DynamiteMesh->SetMaterial(0, DynamicInstance);
}

//calback fired whenever the destructible mesh is destroyed, unused
void AExplosiveActor::OnDynamiteFractured(const FVector& HitPoint, const FVector& HitDirection)
{
	//create explosion and do everything you need to make this super cool
	IgniterParticleEffect->DestroyComponent();
	ExplosiveTypeParticleEffect->DestroyComponent();

	//spawn a blast particle effect on the location this actor was when the mesh fracture happened
	AExplosion* explosionActor = GetWorld()->SpawnActor<AExplosion>(ExplosionBP, GetActorLocation(), FRotator(0, 0, 0));
	explosionActor->ApplyProperties(explosiveType);
	explosionActor->Explode();

	if (explosiveType == EExplosiveType::Fire)
	{
		//reward points && shake the camera effect
		AUnrealShooterLevelScriptActor* MyLvlBP = Cast<AUnrealShooterLevelScriptActor>(GetWorld()->GetLevelScriptActor());
		if (MyLvlBP)
		{
			MyLvlBP->CameraShake();
		}
	}

	GetWorld()->GetTimerManager().SetTimer(ActorTimerHandle, this, &AExplosiveActor::Die, TIME_TO_LIVE, false);
}

void AExplosiveActor::Die()
{
	//clear timer
	GetWorld()->GetTimerManager().ClearTimer(ActorTimerHandle);

	//bye bye
	Destroy();
}
