// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ExplosiveActor.h"
#include "Explosion.generated.h"

UCLASS()
class UNREALSHOOTER_API AExplosion : public AActor
{
	GENERATED_BODY()

private:

	EExplosiveType explosiveType;

	UFUNCTION(Category = "Execution")
		void SpawnParticleEffect();

	UFUNCTION(Category = "Execution")
		void ApplyExplosionEffect();

	UFUNCTION(Category = "Execution")
		void ApplySoundEffect();

	UFUNCTION(Category = "Destroy")
		void Die();
	
public:	
	// Sets default values for this actor's properties
	AExplosion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	const float TIME_TO_LIVE = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		USphereComponent* CollisionSphere;

	UFUNCTION(Category = "Properties")
		void ApplyProperties(EExplosiveType type);

	UFUNCTION(Category = "Execution")
		void Explode();

#pragma region Particle System

private:

	UParticleSystem* FireBlastEffectReference;
	UParticleSystem* IceBlastEffectReference;
	UParticleSystem* LightningBlastEffectReference;
	UParticleSystem* LightningBeamReference;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle System")
		UParticleSystemComponent* BlastParticleEffect;

#pragma endregion

#pragma region Timer
public:
	/* Handle to manage the timer */
	FTimerHandle ActorTimerHandle;

#pragma endregion

#pragma region Sound
public:

	USoundCue* FireExplosionSoundCue;
	USoundCue* IceExplosionSoundCue;
	USoundCue* LightningExplosionSoundCue;

#pragma endregion


/*

//This function acts like a static function that can be accessed from anywhere in the game
FORCEINLINE bool AExplosion::getSomething() const
{
	return true;
}

*/
	
};
