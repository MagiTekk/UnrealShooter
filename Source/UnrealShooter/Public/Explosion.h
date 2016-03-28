// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ExplosiveActor.h"
#include "Explosion.generated.h"

UCLASS()
class UNREALSHOOTER_API AExplosion : public AActor
{
	GENERATED_BODY()

	USoundCue* ExplosionSoundCue;
	
public:	
	// Sets default values for this actor's properties
	AExplosion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		USphereComponent* CollisionSphere;

	UFUNCTION(Category = "Properties")
		void ApplyProperties(EExplosiveType explosiveType);

	UFUNCTION(Category = "Execution")
		void Explode();

	UFUNCTION(Category = "Destroy")
		void Die();

#pragma region Particle System

private:

	UParticleSystem* FireBlastEffectReference;
	UParticleSystem* IceBlastEffectReference;
	UParticleSystem* LightningBlastEffectReference;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle System")
		UParticleSystemComponent* BlastParticleEffect;

#pragma endregion

#pragma region Timer
public:
	/* Handle to manage the timer */
	FTimerHandle ActorTimerHandle;

#pragma endregion
	
};
