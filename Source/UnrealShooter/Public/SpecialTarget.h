// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpecialTarget.generated.h"

USTRUCT()
struct FSpecialTargetProperties
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FVector InitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TArray<FVector> Locations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float TimeToLive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 Points;

	//For GC
	void Destroy()
	{
	}

	FSpecialTargetProperties()
	{
		this->TimeToLive = 5.0f;
	}

	FSpecialTargetProperties(FVector InitialLocation, float TimeToLive, int32 Points, TArray<FVector> Locations = TArray<FVector>{}, float Speed = 1.0f)
	{
		this->InitialLocation = InitialLocation;
		this->Locations = Locations;
		this->Speed = Speed;
		this->TimeToLive = TimeToLive;
		this->Points = Points;
	}
};

UCLASS()
class UNREALSHOOTER_API ASpecialTarget : public AActor
{
	GENERATED_BODY()

	bool bMoveTarget;
	FVector NextLocation;
	FVector GetSpawnPoint(FVector SpawnPosition);

	void SetNewLocation();
	void UpdateTargetLocation();
	
public:	
	// Sets default values for this actor's properties
	ASpecialTarget();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UDestructibleComponent* DestructibleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FSpecialTargetProperties TargetProperties;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
		UMaterialInstanceConstant* DefaultMaterialInst;

	UPROPERTY(BlueprintReadOnly, Category = "Material")
		UMaterialInstanceDynamic* DynamicInstance;

	UFUNCTION(Category = "Properties")
		void UpdateMaterialInstance();

	UFUNCTION(Category = "Properties")
		void ApplyProperties(FSpecialTargetProperties props);

	UFUNCTION(Category = "Destroy")
		void Die();

	UFUNCTION()
		void OnMeshFractured(const FVector& HitPoint, const FVector& HitDirection);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
		FVector initLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
		TArray<FVector> Locations;

	UPROPERTY(BlueprintReadOnly, Category = "Effects")
		UClass* ExplosionBP;

#pragma region Constants

	const FLinearColor DEFAULTTARGET_COLOR = { 0.24f, 0.009788f, 0.0f, 0.0f };

#pragma endregion

#pragma region Particle System

private:

	UParticleSystem* SpecialParticleEffectReference;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle System")
		UParticleSystemComponent* ExplosiveTypeParticleEffect;

#pragma endregion
};
