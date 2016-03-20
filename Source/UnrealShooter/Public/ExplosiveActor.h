// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ExplosiveActor.generated.h"

UCLASS()
class UNREALSHOOTER_API AExplosiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
		UMaterialInstanceConstant* DefaultMaterialInst;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Explosive")
		UParticleSystemComponent* ExplosiveParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Explosive")
		UParticleSystemComponent* ExplosionEffect;
	
};
