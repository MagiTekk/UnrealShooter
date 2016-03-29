// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "RotatableTarget.h"
#include "Explosion.h"


// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	this->SetRootComponent(DefaultSceneRoot);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(365.0f);
	CollisionSphere->AttachTo(RootComponent);

	//Particle Effects
	ConstructorHelpers::FObjectFinder<UParticleSystem> fireBlastEmitter(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	FireBlastEffectReference = fireBlastEmitter.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> iceBlastEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/Ice/Ice_blast.Ice_blast'"));
	IceBlastEffectReference = iceBlastEmitter.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> lightningBlastEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/Lightning/P_DOT_Lightning_01.P_DOT_Lightning_01'"));
	LightningBlastEffectReference = lightningBlastEmitter.Object;

	BlastParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BlastParticleEffect"));
	BlastParticleEffect->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	BlastParticleEffect->AttachTo(RootComponent);

	//Sound
	ConstructorHelpers::FObjectFinder<USoundCue> fireCue(TEXT("SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	FireExplosionSoundCue = fireCue.Object;

	ConstructorHelpers::FObjectFinder<USoundCue> iceCue(TEXT("SoundCue'/Game/UnrealShooter/Audio/Explosions/IceExplosion_Cue.IceExplosion_Cue'"));
	IceExplosionSoundCue = iceCue.Object;

	ConstructorHelpers::FObjectFinder<USoundCue> lightningnCue(TEXT("SoundCue'/Game/UnrealShooter/Audio/Explosions/LightningBolt_Cue.LightningBolt_Cue'"));
	LightningExplosionSoundCue = lightningnCue.Object;
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
}

void AExplosion::Explode()
{
	//play particle effect
	SpawnParticleEffect();

	//play sound
	ApplySoundEffect();

	//get all affected targets and apply explosion
	ApplyExplosionEffect();

	//set a timer for destruction
	GetWorld()->GetTimerManager().SetTimer(ActorTimerHandle, this, &AExplosion::Die, TIME_TO_LIVE, false);
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
			BlastParticleEffect->SetTemplate(IceBlastEffectReference);
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
					Target->Freeze();
					break;
				case EExplosiveType::Lightning:
					Target->OnTargetHit(); //TODO nullbot: change
					break;
				default:
					Target->OnTargetHit();
					break;
			}
		}
	}
}

void AExplosion::ApplySoundEffect()
{
	switch (explosiveType)
	{
		case EExplosiveType::Fire:
			UGameplayStatics::PlaySoundAtLocation(this, FireExplosionSoundCue, GetActorLocation());
			break;
		case EExplosiveType::Ice:
			UGameplayStatics::PlaySoundAtLocation(this, IceExplosionSoundCue, GetActorLocation());
			break;
		case EExplosiveType::Lightning:
			UGameplayStatics::PlaySoundAtLocation(this, LightningExplosionSoundCue, GetActorLocation());
			break;
		default:
			UGameplayStatics::PlaySoundAtLocation(this, FireExplosionSoundCue, GetActorLocation());
			break;
	}
}

void AExplosion::Die()
{
	GetWorld()->GetTimerManager().ClearTimer(ActorTimerHandle);
	Destroy();
}

