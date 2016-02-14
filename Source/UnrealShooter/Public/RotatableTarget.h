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

USTRUCT()
struct FRotatableTargetStruct
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	TArray<FVector> Locations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float TimeToLive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float Points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float HeadshotPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool bIsExplosive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	ETargetType TargetType;

	//For GC
	void Destroy()
	{
	}

	FRotatableTargetStruct()
	{
		this->TimeToLive = 5.0f;
	}

	FRotatableTargetStruct(float TimeToLive, ETargetType TargetType = ETargetType::DefaultTarget, TArray<FVector> Locations = TArray<FVector>{}, float Speed=1.0f, bool bIsExplosive = false)
	{
		this->Locations = Locations;
		this->Speed = Speed;
		this->TimeToLive = TimeToLive;
		this->bIsExplosive = bIsExplosive;
		this->TargetType = TargetType; //defines points rewarded per hit & target color
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

	float RotationalRate = DEFAULT_ROTATIONAL_RATE;

	FVector NextLocation;
	void SetNewLocation();

	void InitMaterialInstance();
	FLinearColor GetMaterialColor();

	void ApplyProperties();
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

	void SetTargetProperties(FRotatableTargetStruct TargetProperties);

	void OnPropertiesUpdated();

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
	FRotatableTargetStruct TargetProperties;

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
