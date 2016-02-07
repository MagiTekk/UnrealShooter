// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "RotatableTarget.generated.h"

UCLASS()
class UNREALSHOOTER_API ARotatableTarget : public AActor
{
	GENERATED_BODY()

	bool bRaiseTarget;
	bool bLowerTarget;
	bool bVanish;

	void DoTargetUp();
	void DoTargetDown();
	void Vanish();
	void Die();

protected:

	const float ROTATIONAL_RATE = 1.0f;
	const float LOWERED_ROTATION = -90.0f;
	const float RAISED_ROTATION = 0.0f;
	
public:	
	// Sets default values for this actor's properties
	ARotatableTarget();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Target)
	UDestructibleComponent* HeadMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Target)
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	UMaterial* BaseMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Materials")
	UMaterialInstanceDynamic* DynamicInstance;

	void RaiseTarget();
	void LowerTarget();
	void Kill();

#pragma region Timer

	/* Handle to manage the timer */
	FTimerHandle TargetTimerHandle;

#pragma endregion
};
