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

#pragma region ColorCodes

	const FLinearColor FIRE_ELEMENT =		{ 0.335f, 0.081f, 0.019f, 0.02f };
	const FLinearColor ICE_ELEMENT =		{ 0.0f, 1.0f, 0.335f, 0.323f };
	const FLinearColor LIGHTNING_ELEMENT =	{ 0.283f, 0.3f, 1.0f, 0.335f };
	const FLinearColor DEFAULT_ELEMENT = { 0.335f, 0.335f, 0.019f, 0.02f };

#pragma endregion

#pragma region Material Instances

	UPROPERTY(EditDefaultsOnly, Category = "Material")
		UMaterialInstanceConstant* DefaultMaterialInst;

	UPROPERTY(BlueprintReadOnly, Category = "Material")
		UMaterialInstanceDynamic* DynamicInstance;

#pragma endregion

#pragma region Particle System

private:

	UParticleSystem* FireParticleEffectReference;
	//UParticleSystem FireBlastEffectReference;
	
	/*ConstructorHelpers::FObjectFinder<UParticleSystem> IceParticleEffectReference;
	ConstructorHelpers::FObjectFinder<UParticleSystem> IceBlastEffectReference;

	ConstructorHelpers::FObjectFinder<UParticleSystem> LightningParticleEffectReference;
	ConstructorHelpers::FObjectFinder<UParticleSystem> LightningBlastEffectReference;*/

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle System")
		UParticleSystemComponent* IgniterParticleEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle System")
		UParticleSystemComponent* ExplosiveTypeParticleEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle System")
		UParticleSystemComponent* BlastParticleEffect;

#pragma endregion

};
