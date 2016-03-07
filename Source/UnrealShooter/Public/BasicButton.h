// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UnrealShooterLevelScriptActor.h"
#include "BasicButton.generated.h"

UCLASS()
class UNREALSHOOTER_API ABasicButton : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY()
		UMaterialInstanceDynamic* MaterialInstanceButton;

	UPROPERTY()
		bool bIsActive;
	
public:	
	// Sets default values for this actor's properties
	ABasicButton();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* ButtonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
		UCapsuleComponent* TriggerCollission;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Text")
		UTextRenderComponent* TextRenderComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
		ESequenceEnum SequenceType;

	UPROPERTY(BlueprintReadOnly, Category = "Material")
		UMaterialInstanceDynamic* DynamicMaterialInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Material")
		UMaterialInstanceConstant* DynamicInstanceConstant;

	UFUNCTION()
		virtual void OnContextAction();
};
