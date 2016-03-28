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

	ConstructorHelpers::FObjectFinder<UParticleSystem> igniterEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/IgniterEffect.IgniterEffect'"));
	IgniterParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IgniterParticleEffect"));
	IgniterParticleEffect->SetTemplate(igniterEmitter.Object);
	IgniterParticleEffect->AttachTo(RootComponent);

	//get all particle effects and assign one of them on apply properties
	ConstructorHelpers::FObjectFinder<UParticleSystem> fireEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/Fire/P_Fire_Torch_01.P_Fire_Torch_01'"));
	FireParticleEffectReference = fireEmitter.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> iceEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/Ice/Ice_effect.Ice_effect'"));
	IceParticleEffectReference = iceEmitter.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> lightningEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/Lightning/P_DOT_Lightning_01.P_DOT_Lightning_01'"));
	LightningParticleEffectReference = lightningEmitter.Object;

	ExplosiveTypeParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosiveTypeParticleEffect"));
	ExplosiveTypeParticleEffect->AttachTo(RootComponent);

	//ConstructorHelpers::FObjectFinder<UParticleSystem> fireBlastEmitter(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	//FireParticleEffectReference = FireBlastEffectReference;

	//BlastParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BlastParticleEffect"));
	//BlastParticleEffect->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> DefaultMaterialObj(TEXT("MaterialInstanceConstant'/Game/UnrealShooter/Materials/Instances/Targets/BasicMaterial_Inst_DefaultTarget.BasicMaterial_Inst_DefaultTarget'"));
	DefaultMaterialInst = DefaultMaterialObj.Object;

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
	ApplyProperties(EExplosiveType::Default);
}

void AExplosiveActor::ApplyProperties(EExplosiveType explosiveType)
{
	//DynamicInstance = UMaterialInstanceDynamic::Create(DefaultMaterialInst, this);
	switch (explosiveType)
	{
		case EExplosiveType::Fire:
			DynamicInstance = UMaterialInstanceDynamic::Create(FireMaterialInst, this);
			break;
		case EExplosiveType::Ice:
			DynamicInstance = UMaterialInstanceDynamic::Create(IceMaterialInst, this);
			break;
		case EExplosiveType::Lightning:
			DynamicInstance = UMaterialInstanceDynamic::Create(LightningMaterialInst, this);
			break;
		default:
			//DynamicInstance = UMaterialInstanceDynamic::Create(DefaultMaterialInst, this);
			DynamicInstance = UMaterialInstanceDynamic::Create(LightningMaterialInst, this);
			//DynamicInstance->SetVectorParameterValue("BaseColor", DEFAULT_ELEMENT_COLOR);
			break;
	}

	DynamiteMesh->SetMaterial(0, DynamicInstance);
	//ExplosiveTypeParticleEffect->SetTemplate(FireParticleEffectReference);
	ExplosiveTypeParticleEffect->SetTemplate(LightningParticleEffectReference);
}

//calback fired whenever the destructible mesh is destroyed, unused
void AExplosiveActor::OnDynamiteFractured(const FVector& HitPoint, const FVector& HitDirection)
{
	//create explosion and do everything you need to make this super cool
	IgniterParticleEffect->DestroyComponent();
	ExplosiveTypeParticleEffect->DestroyComponent();

	//spawn a blast particle effect on the location this actor was when the mesh fracture happened

	GetWorld()->GetTimerManager().SetTimer(ActorTimerHandle, this, &AExplosiveActor::Die, 2.0f, false);
}

void AExplosiveActor::Die()
{
	//clear timer
	GetWorld()->GetTimerManager().ClearTimer(ActorTimerHandle);

	//bye bye
	Destroy();
}
