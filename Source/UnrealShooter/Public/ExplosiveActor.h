// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ExplosiveActor.generated.h"

UENUM()
enum class EExplosiveType : uint8
{
	Fire,
	Ice,
	Lightning,
	Default
};

UCLASS()
class UNREALSHOOTER_API AExplosiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AExplosiveActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UDestructibleComponent* DynamiteMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		USphereComponent* CollisionSphere;

	UFUNCTION(Category = "Properties")
		void ApplyProperties(EExplosiveType explosiveType);

	UFUNCTION(Category = "Destroy")
		void Die();

	UFUNCTION()
		void OnDynamiteFractured(const FVector& HitPoint, const FVector& HitDirection);

#pragma region ColorCodes

	const FLinearColor DEFAULT_ELEMENT_COLOR = { 0.335f, 0.335f, 0.019f, 0.02f };

#pragma endregion

#pragma region Material Instances

	UPROPERTY(EditDefaultsOnly, Category = "Material")
		UMaterialInstanceConstant* DefaultMaterialInst;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
		UMaterialInstanceConstant* FireMaterialInst;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
		UMaterialInstanceConstant* IceMaterialInst;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
		UMaterialInstanceConstant* LightningMaterialInst;

	UPROPERTY(BlueprintReadOnly, Category = "Material")
		UMaterialInstanceDynamic* DynamicInstance;

#pragma endregion

#pragma region Particle System

private:

	UParticleSystem* FireParticleEffectReference;
	UParticleSystem* IceParticleEffectReference;
	UParticleSystem* LightningParticleEffectReference;

	UParticleSystem* FireBlastEffectReference;
	UParticleSystem* IceBlastEffectReference;
	UParticleSystem* LightningBlastEffectReference;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle System")
		UParticleSystemComponent* IgniterParticleEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle System")
		UParticleSystemComponent* ExplosiveTypeParticleEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle System")
		UParticleSystemComponent* BlastParticleEffect;

#pragma endregion

#pragma region Timer
public:
	/* Handle to manage the timer */
	FTimerHandle ActorTimerHandle;

#pragma endregion

};
