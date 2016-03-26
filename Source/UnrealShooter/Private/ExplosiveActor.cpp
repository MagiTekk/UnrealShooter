// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "ExplosiveActor.h"
#include "Engine/DestructibleMesh.h"


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
	DynamiteMesh->AttachTo(DefaultSceneRoot);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(365.0f);
	CollisionSphere->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<UParticleSystem> igniterEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/IgniterEffect.IgniterEffect'"));
	IgniterParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IgniterParticleEffect"));
	IgniterParticleEffect->SetTemplate(igniterEmitter.Object);
	IgniterParticleEffect->AttachTo(RootComponent);

	//get all particle effects and assign one of them on apply properties
	ConstructorHelpers::FObjectFinder<UParticleSystem> fireEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/Fire/P_Fire_Torch_01.P_Fire_Torch_01'"));
	FireParticleEffectReference = fireEmitter.Object;
	//ConstructorHelpers::FObjectFinder<UParticleSystem> fireBlastEmitter(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	//FireParticleEffectReference = FireBlastEffectReference;

	ExplosiveTypeParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosiveTypeParticleEffect"));
	ExplosiveTypeParticleEffect->AttachTo(RootComponent);

	//BlastParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BlastParticleEffect"));
	//BlastParticleEffect->AttachTo(RootComponent);

	/*ConstructorHelpers::FObjectFinder<UParticleSystem> fireBlastEmitter(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	ExplosiveTypeParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosiveTypeParticleEffect"));
	//ExplosiveTypeParticleEffect->SetTemplate(explosiveEmitter.Object);
	ExplosiveTypeParticleEffect->AttachTo(RootComponent);*/

	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> DefaultMaterialObj(TEXT("MaterialInstanceConstant'/Game/UnrealShooter/Materials/Instances/Targets/BasicMaterial_Inst_DefaultTarget.BasicMaterial_Inst_DefaultTarget'"));
	DefaultMaterialInst = DefaultMaterialObj.Object;
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
	ApplyProperties(EExplosiveType::Default);
}

void AExplosiveActor::ApplyProperties(EExplosiveType explosiveType)
{
	DynamicInstance = UMaterialInstanceDynamic::Create(DefaultMaterialInst, this);
	switch (explosiveType)
	{
		case EExplosiveType::Fire:
			DynamicInstance->SetVectorParameterValue("BaseColor", FIRE_ELEMENT);
			break;
		case EExplosiveType::Ice:
			DynamicInstance->SetVectorParameterValue("BaseColor", ICE_ELEMENT);
			break;
		case EExplosiveType::Lightning:
			DynamicInstance->SetVectorParameterValue("BaseColor", LIGHTNING_ELEMENT);
			break;
		default:
			DynamicInstance->SetVectorParameterValue("BaseColor", DEFAULT_ELEMENT);
			break;
	}
	DynamiteMesh->SetMaterial(0, DynamicInstance);
}

