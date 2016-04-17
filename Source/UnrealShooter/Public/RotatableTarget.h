// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ExplosiveActor.h"
#include "RotatableTarget.generated.h"

UENUM()
enum class ETargetType : uint8
{
	InnocentTarget,
	FemaleTarget,
	MaleTarget
};

USTRUCT()
struct FTargetTypeProperties
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 TargetTypeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		ETargetType TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 Points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 HeadShotPoints;

	FTargetTypeProperties()
	{
	}

	FTargetTypeProperties(int32 TargetTypeID, ETargetType TargetType, int32 Points, int32 HeadShotPoints)
	{
		this->TargetTypeID = TargetTypeID;
		this->TargetType = TargetType;
		this->Points = Points;
		this->HeadShotPoints = HeadShotPoints;
	}
};

USTRUCT()
struct FTargetLocation
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 LocationID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FVector Location;

	FTargetLocation()
	{
	}

	FTargetLocation(int32 LocationID, FVector location)
	{
		this->LocationID = LocationID;
		this->Location = location;
	}
};

USTRUCT()
struct FRotatableTargetProperties
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 TargetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FVector InitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TArray<FVector> Locations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 Points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 HeadshotPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		EExplosiveType ExplosiveType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		ETargetType TargetType;

	//For GC
	void Destroy()
	{
	}

	FRotatableTargetProperties()
	{

	}

	FRotatableTargetProperties(int32 TargetID, FVector InitialLocation, int32 Points, int32 HeadshotPoints, ETargetType TargetType = ETargetType::MaleTarget, TArray<FVector> Locations = TArray<FVector>{}, float Speed = 1.0f, EExplosiveType ExplosiveType = EExplosiveType::NonExplosive)
	{
		this->TargetID = TargetID;
		this->InitialLocation = InitialLocation;
		this->Locations = Locations;
		this->Speed = Speed;
		this->Points = Points;
		this->HeadshotPoints = HeadshotPoints;
		this->ExplosiveType = ExplosiveType;
		this->TargetType = TargetType;
	}
};

UCLASS()
class UNREALSHOOTER_API ARotatableTarget : public AActor
{
	GENERATED_BODY()

	bool bRaiseTarget;
	bool bLowerTarget;
	bool bVanish;
	bool bMoveTarget;
	bool bIsTranslucent;

	float RotationalRate = DEFAULT_ROTATIONAL_RATE;

	FVector NextLocation;
	void SetNewLocation();

	void InitMaterialInstance();
	FLinearColor GetMaterialColor();

	FVector GetSpawnPoint(FVector SpawnPosition);
	
	void DoTargetUp();
	void DoTargetDown();
	void Vanish();
	void Die();
	void UpdateTargetLocation();
	void RewardTargetPoints();

protected:

	const float DEFAULT_ROTATIONAL_RATE =			5.0f; //2.0 at 120fps
	const float LOWERED_ROTATION =					-90.0f;
	const float RAISED_ROTATION =					0.0f;

	const float FROZEN_TIME = 10.0f;
	const float LiGHTNING_TIME = 3.0f;

	//color codes
	const FLinearColor DEFAULTTARGET_COLOR =		{ 0.1f, 0.1f, 0.1f, 0.0f };
	const FLinearColor FALSETARGET_COLOR =			{ 1.0f, 0.066f, 0.019f, 0.0f };
	const FLinearColor MIDTARGET_COLOR =			{ 0.073f, 1.0f, 0.149f, 0.0f };
	const FLinearColor LOWTARGET_COLOR =			{ 0.064f, 0.169f, 1.0f, 0.0f };
	
public:	
	// Sets default values for this actor's properties
	ARotatableTarget();

	void InitTarget();
	void InitDynamite();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	void ApplyProperties(FRotatableTargetProperties TargetProperties, int32 TimeToLive);

	void UpdateMaterialInstance();

	bool bIsFrozen;
	bool bIsElectrified;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scene")
		USceneComponent* HeadCenterPointScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UDestructibleComponent* HeadMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UDestructibleComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UChildActorComponent* Dynamite;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
		UMaterialInstanceConstant* DefaultMaterialInst;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
		UMaterialInstanceConstant* TransparentMaterialInst;

	UPROPERTY(BlueprintReadOnly, Category = "Material")
		UMaterialInstanceDynamic* DynamicInstance;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Target")
		FVector CustomPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		FRotatableTargetProperties TargetProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int32 TimeToLive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bIsHeadShot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		bool bPointsRedeemed;

	UFUNCTION()
		void OnHeadFractured(const FVector& HitPoint, const FVector& HitDirection);

	UFUNCTION()
		void OnTargetHit(bool forceHit = false);
	
	UFUNCTION()
		void RaiseTarget();
	
	UFUNCTION()
		void LowerTarget();
	
	UFUNCTION()
		void startVanish();

	UFUNCTION()
		void Freeze();

	UFUNCTION()
		void UnFreeze();

	UFUNCTION()
		void LightningIncoming();

	UFUNCTION()
		void LightningStrike();

	UFUNCTION()
		void ChainHitsCheck();

	/* Used to vanish a target even when it wasn't hit*/
	UFUNCTION()
		void FakeTargetHit();

#pragma region Timer
public:

	/* Handle to manage the timer */
	FTimerHandle TargetTimerHandle;
	FTimerHandle FreezeTimerHandle;
	FTimerHandle LightningTimerHandle;

#pragma endregion
};
