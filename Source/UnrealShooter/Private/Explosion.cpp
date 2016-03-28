// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "RotatableTarget.h"
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

void AExplosion::ApplyProperties(EExplosiveType type)
{
	explosiveType = type;

	/*switch (explosiveType) //use sound component to set the explosion sound
	{
		case EExplosiveType::Fire:
			BlastParticleEffect->SetTemplate(FireBlastEffectReference);
			break;
		case EExplosiveType::Ice:
			BlastParticleEffect->SetTemplate(IceBlastEffectReference);
			break;
		case EExplosiveType::Lightning:
			BlastParticleEffect->SetTemplate(LightningBlastEffectReference);
			break;
		default:
			BlastParticleEffect->SetTemplate(FireBlastEffectReference);
			break;
	}*/

	//Explode();
}

void AExplosion::Explode()
{
	//play particle effect
	SpawnParticleEffect();

	//play sound
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSoundCue, GetActorLocation());

	//get all affected targets and apply explosion
	ApplyExplosionEffect();

	//set a timer for destruction
	GetWorld()->GetTimerManager().SetTimer(ActorTimerHandle, this, &AExplosion::Die, 4.0f, false);
}

void AExplosion::SpawnParticleEffect()
{
	switch (explosiveType)
	{
		case EExplosiveType::Fire:
			BlastParticleEffect->SetTemplate(FireBlastEffectReference);
			break;
		case EExplosiveType::Ice:
			BlastParticleEffect->SetTemplate(IceBlastEffectReference);
			break;
		case EExplosiveType::Lightning:
			BlastParticleEffect->SetTemplate(LightningBlastEffectReference);
			break;
		default:
			BlastParticleEffect->SetTemplate(FireBlastEffectReference);
			break;
	}
}

void AExplosion::ApplyExplosionEffect()
{
	//Get all overlapping actors and store them in a CollectedActors array
	TArray<AActor*> CollectedActors;
	CollisionSphere->GetOverlappingActors(CollectedActors);

	//for each Actor collected
	for (int32 i = 0; i < CollectedActors.Num(); i++)
	{
		AActor * ACollected = CollectedActors[i];

		//Cast the collected Actor to ABatteryPickup
		ARotatableTarget * const Target = Cast<ARotatableTarget>(ACollected);

		//if the cast is succesful
		if (Target)
		{
			//apply explosion effect to target, fire target blows them up, ice freezes them, lightning spawns a ray to each target's head
			
			switch (explosiveType)
			{
				case EExplosiveType::Fire:
					Target->OnTargetHit();
					break;
				case EExplosiveType::Ice:
					//BlastParticleEffect->SetTemplate(IceBlastEffectReference);
					break;
				case EExplosiveType::Lightning:
					//BlastParticleEffect->SetTemplate(LightningBlastEffectReference);
					break;
				default:
					Target->OnTargetHit();
					break;
			}
		}
	}
}

void AExplosion::Die()
{
	GetWorld()->GetTimerManager().ClearTimer(ActorTimerHandle);
	Destroy();
}

