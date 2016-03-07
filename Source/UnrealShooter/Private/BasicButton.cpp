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

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRenderComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponent->SetVisibility(false);
	TextRenderComponent->AttachTo(DefaultSceneRoot);

	ConstructorHelpers::FObjectFinder<UStaticMesh> meshBody(TEXT("StaticMesh'/Game/DemoRoom/Meshes/SM_Button.SM_Button'"));
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetStaticMesh(meshBody.Object);
	ButtonMesh->AttachTo(DefaultSceneRoot);

	TriggerCollission = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerCollission"));
	TriggerCollission->AttachTo(ButtonMesh);
	TriggerCollission->SetCapsuleSize(156.53f, 156.53f);

	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> DefaultMaterialObj(TEXT("MaterialInstanceConstant'/Game/DemoRoom/Materials/M_Button_Inst.M_Button_Inst'"));
	DynamicInstanceConstant = DefaultMaterialObj.Object;

	bIsActive = false;
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

void ABasicButton::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(DynamicInstanceConstant, this);
	DynamicMaterialInstance->SetScalarParameterValue("State", 1);
	ButtonMesh->SetMaterial(2, DynamicMaterialInstance);
}

void ABasicButton::NotifyActorBeginOverlap(AActor * OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Button - Enter Overlap"));
	TextRenderComponent->SetVisibility(true);

	//fire signal
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnActorBeginOverlap.Broadcast(this);
}

void ABasicButton::NotifyActorEndOverlap(AActor * OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Button - Exit Overlap"));
	TextRenderComponent->SetVisibility(false);

	//set material instance state to active
	DynamicMaterialInstance->SetScalarParameterValue("State", 1);
	bIsActive = false;

	//fire signal
	UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
	DataInstance->OnActorEndOverlap.Broadcast();
}

void ABasicButton::OnContextAction()
{
	if (!bIsActive)
	{
		//set material instance state to active
		DynamicMaterialInstance->SetScalarParameterValue("State", 0);

		AUnrealShooterLevelScriptActor* MyLvlBP = Cast<AUnrealShooterLevelScriptActor>(GetWorld()->GetLevelScriptActor());

		if (!MyLvlBP) return;
		MyLvlBP->PlaySequence(SequenceType);

		bIsActive = true;
	}
}

