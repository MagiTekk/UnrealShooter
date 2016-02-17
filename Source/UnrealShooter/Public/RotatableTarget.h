// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UnrealShooterEnumData.h"
#include "UnrealShooterStructData.h"
#include "RotatableTarget.generated.h"

UCLASS()
class UNREALSHOOTER_API ARotatableTarget : public AActor
{
	GENERATED_BODY()

	bool bRaiseTarget;
	bool bLowerTarget;
	bool bVanish;
	bool bMoveTarget;

	float RotationalRate = DEFAULT_ROTATIONAL_RATE;

	FVector NextLocation;
	void SetNewLocation();

	void InitMaterialInstance();
	FLinearColor GetMaterialColor();

	
	void DoTargetUp();
	void DoTargetDown();
	void Vanish();
	void Die();
	void UpdateTargetLocation();

protected:

	const float DEFAULT_ROTATIONAL_RATE =			1.0f;
	const float LOWERED_ROTATION =			90.0f;
	const float RAISED_ROTATION =			0.0f;

	//color codes
	const FLinearColor DEFAULTTARGET_COLOR =		{ 1.0f, 1.0f, 1.0f, 0.0f };
	const FLinearColor FALSETARGET_COLOR =			{ 1.0f, 0.066f, 0.019f, 0.0f };
	const FLinearColor MIDTARGET_COLOR =			{ 0.073f, 1.0f, 0.149f, 0.0f };
	const FLinearColor LOWTARGET_COLOR =			{ 0.064f, 0.169f, 1.0f, 0.0f };
	
public:	
	// Sets default values for this actor's properties
	ARotatableTarget();

	void InitTarget();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	void ApplyProperties(FRotatableTargetProperties TargetProperties);

	void UpdateMaterialInstance(bool bisTranslucent = false);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UDestructibleComponent* HeadMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly, Category = Material)
	UMaterialInstanceConstant* DefaultMaterialInst;

	UPROPERTY(EditDefaultsOnly, Category = Material)
	UMaterialInstanceConstant* TransparentMaterialInst;

	UPROPERTY(BlueprintReadOnly, Category = Material)
	UMaterialInstanceDynamic* DynamicInstance;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Target)
	ETargetType TargetType;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Target)
	FVector CustomPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	FRotatableTargetProperties TargetProperties;

	//UFUNCTION()
	//void OnHeadFractured(const FVector& HitPoint, const FVector& HitDirection);

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
