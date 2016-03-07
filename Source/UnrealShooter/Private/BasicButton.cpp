// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "BasicButton.h"


// Sets default values
ABasicButton::ABasicButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	this->SetRootComponent(DefaultSceneRoot);

	ConstructorHelpers::FObjectFinder<UStaticMesh> meshBody(TEXT("StaticMesh'/Game/DemoRoom/Meshes/SM_Button.SM_Button'"));
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetStaticMesh(meshBody.Object);
	ButtonMesh->AttachTo(DefaultSceneRoot);

	TriggerCollission = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerCollission"));
	TriggerCollission->AttachTo(ButtonMesh);
	TriggerCollission->SetCapsuleSize(156.53f, 156.53f);
}

// Called when the game starts or when spawned
void ABasicButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicButton::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABasicButton::NotifyActorBeginOverlap(AActor * OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Button - Enter Overlap"));
	//set material instance state to active
	//ButtonMesh->

	//fire signal
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnActorBeginOverlap.Broadcast(this);
}

void ABasicButton::NotifyActorEndOverlap(AActor * OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Button - Exit Overlap"));
	//set material instance state to inactive

	//fire signal
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnActorEndOverlap.Broadcast();
}

void ABasicButton::OnContextAction()
{
	AUnrealShooterLevelScriptActor* MyLvlBP = Cast<AUnrealShooterLevelScriptActor>(GetWorld()->GetLevelScriptActor());

	if (!MyLvlBP) return;
	MyLvlBP->PlaySequence(SequenceType);
}

