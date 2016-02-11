// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "RotatableTarget.generated.h"

UENUM()
enum class ETargetType : uint8
{
	DefaultTarget,
	FalseTarget,
	MidTarget,
	LowTarget
};

UCLASS()
class UNREALSHOOTER_API ARotatableTarget : public AActor
{
	GENERATED_BODY()

	bool bHeadDestroyed;
	bool bRaiseTarget;
	bool bLowerTarget;
	bool bVanish;

	void InitMaterialInstance();
	UMaterialInstanceConstant* GetMaterialInstance();

	void DoTargetUp();
	void DoTargetDown();
	void Vanish();
	void Die();

protected:

	const float ROTATIONAL_RATE = 1.0f;
	const float LOWERED_ROTATION = 90.0f;
	const float RAISED_ROTATION = 0.0f;
	
public:	
	// Sets default values for this actor's properties
	ARotatableTarget();
	ARotatableTarget(ETargetType targetType);

	void InitTarget();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UDestructibleComponent* HeadMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly, Category = Material)
	UMaterialInstanceConstant* defaultMaterialInst;

	UPROPERTY(EditDefaultsOnly, Category = Material)
	UMaterialInstanceConstant* lowtMaterialInst;

	UPROPERTY(EditDefaultsOnly, Category = Material)
	UMaterialInstanceConstant* midMaterialInst;

	UPROPERTY(EditDefaultsOnly, Category = Material)
	UMaterialInstanceConstant* falseMaterialInst;

	UPROPERTY(BlueprintReadOnly, Category = Material)
	UMaterialInstanceDynamic* DynamicInstance;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Target)
	ETargetType TargetType;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Target)
	FVector CustomPosition;

	UFUNCTION()
	void OnHeadFractured(const FVector& HitPoint, const FVector& HitDirection);

	void OnTargetHit();
	void RaiseTarget();
	void LowerTarget();
	void startVanish();

#pragma region Timer
public:
	/* Handle to manage the timer */
	FTimerHandle TargetTimerHandle;

#pragma endregion
};
