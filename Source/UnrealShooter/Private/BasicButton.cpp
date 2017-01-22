// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "BasicButton.h"
#include "UnrealPlayerController.h"
#include "MainCharacter.h"


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
	TextRenderComponent->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepRelativeTransform);

	ConstructorHelpers::FObjectFinder<UStaticMesh> meshBody(TEXT("StaticMesh'/Game/DemoRoom/Meshes/SM_Button.SM_Button'"));
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetStaticMesh(meshBody.Object);
	ButtonMesh->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerCollission = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerCollission"));
	TriggerCollission->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
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
	DynamicMaterialInstance->SetScalarParameterValue("State", 0);
	ButtonMesh->SetMaterial(2, DynamicMaterialInstance);
}

void ABasicButton::NotifyActorBeginOverlap(AActor * OtherActor)
{
	AMainCharacter* mainChar = Cast<AMainCharacter>(OtherActor);
	if (!bIsActive && mainChar)
	{
		ActivateButton();
	}
}

void ABasicButton::NotifyActorEndOverlap(AActor * OtherActor)
{
	AMainCharacter* mainChar = Cast<AMainCharacter>(OtherActor);
	if (!bIsActive && mainChar)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Button - Exit Overlap"));
		TextRenderComponent->SetVisibility(false);

		//set material instance state to active
		DynamicMaterialInstance->SetScalarParameterValue("State", 0);
		//bIsActive = false;

		//fire signal
		UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
		DataInstance->OnActorEndOverlap.Broadcast();
	}
}

/*Only Activate the button if the player is in range*/
void ABasicButton::ActivateButton()
{
	TArray<AActor*> CollectedActors;
	TriggerCollission->GetOverlappingActors(CollectedActors);

	bIsActive = false;
	//for each Actor collected
	for (int32 i = 0; i < CollectedActors.Num(); i++)
	{
		AActor* ACollected = CollectedActors[i];
		AMainCharacter* player = Cast<AMainCharacter>(ACollected);
		if (player)
		{
			TextRenderComponent->SetVisibility(true);
			DynamicMaterialInstance->SetScalarParameterValue("State", 1);

			//fire signal
			UUnrealShooterDataSingleton* DataInstance = Cast<UUnrealShooterDataSingleton>(GEngine->GameSingleton);
			DataInstance->OnActorBeginOverlap.Broadcast(this);
		}
	}
}

void ABasicButton::OnContextAction()
{
	if (!bIsActive)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			AUnrealPlayerController* const UPC = CastChecked<AUnrealPlayerController>(PC);
			if (UPC)
			{
				//set material instance state to active
				DynamicMaterialInstance->SetScalarParameterValue("State", 0);
				bIsActive = true;
				TextRenderComponent->SetVisibility(false);
				UPC->ShowSequenceSelector();
			}
		}
	}
}

