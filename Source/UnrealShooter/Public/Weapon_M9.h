// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon_M9.generated.h"

UCLASS()
class UNREALSHOOTER_API AWeapon_M9 : public AActor
{
	GENERATED_BODY()

	UFUNCTION()
		void MyOverlapFunction(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void UpdateLaserPointer();

public:

	UPROPERTY()
		bool bIsWeaponAttached;

	// Sets default values for this actor's properties
	AWeapon_M9();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
		int32 Ammo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
		int32 AmmoCapacity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaserSight")
		UChildActorComponent* LaserSource;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaserSight")
		UPointLightComponent* LaserImpact;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaserSight")
		UParticleSystemComponent* LaserBeam;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LaserSight")
		FVector LaserTargetLocation;

	UFUNCTION()
		void AddWeapon(USkeletalMeshComponent* MeshComponent, FName SocketName);

	UFUNCTION()
		void DestroyWeapon();

	UFUNCTION()
		void Reload(int32 AmmoToReload);

#pragma region Shooting

	//no more blueprint spawning
	//TSubclassOf<class AActor> BulletBlueprint;


	void ShootWeapon();
	void OnHit(FHitResult hitResult);
	bool singleLineTrace(FHitResult &HitResult);

	UClass* Gunshot_InitialEffect;
	UClass* Gunshot_FinalEffect;
	UClass* Empty_Shell;
#pragma endregion
};
