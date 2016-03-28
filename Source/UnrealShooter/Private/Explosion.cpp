// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "Explosion.h"


// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	this->SetRootComponent(DefaultSceneRoot);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(365.0f);
	CollisionSphere->AttachTo(RootComponent);

	//get all particle effects and assign one of them on apply properties
	ConstructorHelpers::FObjectFinder<UParticleSystem> fireBlastEmitter(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	FireBlastEffectReference = fireBlastEmitter.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> iceBlastEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/Ice/Ice_blast.Ice_blast'"));
	IceBlastEffectReference = iceBlastEmitter.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> lightningBlastEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/Lightning/P_DOT_Lightning_01.P_DOT_Lightning_01'"));
	LightningBlastEffectReference = lightningBlastEmitter.Object;

	BlastParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BlastParticleEffect"));
	BlastParticleEffect->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	BlastParticleEffect->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<USoundCue> explosionCue(TEXT("SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	ExplosionSoundCue = explosionCue.Object;

}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosion::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AExplosion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ApplyProperties(EExplosiveType::Default);
}

void AExplosion::ApplyProperties(EExplosiveType explosiveType)
{
	switch (explosiveType)
	{
		case EExplosiveType::Fire:
			break;
		case EExplosiveType::Ice:
			break;
		case EExplosiveType::Lightning:
			break;
		default:
			break;
	}
	Explode();
}

void AExplosion::Explode()
{
	BlastParticleEffect->SetTemplate(FireBlastEffectReference);

	//play sound
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSoundCue, GetActorLocation());

	GetWorld()->GetTimerManager().SetTimer(ActorTimerHandle, this, &AExplosion::Die, 4.0f, false);
}

void AExplosion::Die()
{
	GetWorld()->GetTimerManager().ClearTimer(ActorTimerHandle);
	Destroy();
}

