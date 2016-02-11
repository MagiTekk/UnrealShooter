// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "Weapon_M9.h"
#include "MainCharacter.h"
#include "RotatableTarget.h"

// Sets default values
AWeapon_M9::AWeapon_M9()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsWeaponAttached = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	this->SetRootComponent(DefaultSceneRoot);

	//Create our overlap sphere component
	/*CollisionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("CollisionSphere"));
	CollisionSphere->AttachTo(RootComponent);
	CollisionSphere->SetSphereRadius(32.f);*/

	//binding the sphere component to my overlap callback
	//CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon_M9::MyOverlapFunction);

	ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh_M9(TEXT("StaticMesh'/Game/UnrealShooter/Mesh/M9/M9.M9'"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetStaticMesh(WeaponMesh_M9.Object);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->AttachTo(DefaultSceneRoot);

	LaserSource = CreateDefaultSubobject<UChildActorComponent>(TEXT("LaserSource"));
	LaserSource->SetRelativeLocation(FVector(10.0f, 0.0f, 4.0f));
	LaserSource->AttachTo(Mesh);

	LaserImpact = CreateDefaultSubobject<UPointLightComponent>(TEXT("LaserImpact"));
	LaserImpact->SetIntensity(20.0f);
	LaserImpact->SetLightColor(FLinearColor(255, 0, 0));
	LaserImpact->SetAttenuationRadius(20.0f);
	LaserImpact->AttachTo(LaserSource);

	ConstructorHelpers::FObjectFinder<UParticleSystem> particleEmitter(TEXT("ParticleSystem'/Game/UnrealShooter/Particles/LaserBeam.LaserBeam'"));
	LaserBeam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserBeam"));
	LaserBeam->SetTemplate(particleEmitter.Object);
	LaserBeam->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<UClass> GunshotInitial(TEXT("Class'/Game/UnrealShooter/Blueprint/M9/Blueprint_Effect_Muzzle_Flash.Blueprint_Effect_Muzzle_Flash_C'"));
	if (GunshotInitial.Object)
	{
		Gunshot_InitialEffect = GunshotInitial.Object;// ->GeneratedClass;
	}

	ConstructorHelpers::FObjectFinder<UClass> GunshotFinal(TEXT("Class'/Game/UnrealShooter/Blueprint/M9/Blueprint_Effect_Bullet_Hit.Blueprint_Effect_Bullet_Hit_C'"));
	if (GunshotFinal.Object)
	{
		Gunshot_FinalEffect = GunshotFinal.Object;// ->GeneratedClass;
	}

	ConstructorHelpers::FObjectFinder<UClass> BulletShell(TEXT("Class'/Game/UnrealShooter/Blueprint/M9/EmptyShell.EmptyShell_C'"));
	if (BulletShell.Object)
	{
		Empty_Shell = BulletShell.Object;// ->GeneratedClass;
	}

	//Spawning a blueprint
	/*ConstructorHelpers::FObjectFinder<UBlueprint> M9Bullet(TEXT("Blueprint'/Game/UnrealShooter/Blueprint/Projectile/M9Bullet.M9Bullet'"));
	if (M9Bullet.Object)
	{
		BulletBlueprint = (UClass*)M9Bullet.Object->GeneratedClass;
	}*/
}

// Called when the game starts or when spawned
void AWeapon_M9::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon_M9::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	AWeapon_M9::UpdateLaserPointer();
}

void AWeapon_M9::MyOverlapFunction(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && !bIsWeaponAttached)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Overlap works!"));
		//AttachRootComponentTo(MeshComponent, SocketName, EAttachLocation::SnapToTarget);
	}
}

void AWeapon_M9::AddWeapon(USkeletalMeshComponent* MeshComponent, FName SocketName)
{
	UWorld * const World = GetWorld();
	if (World)
	{
		// snap to the socket
		AttachRootComponentTo(MeshComponent, SocketName, EAttachLocation::SnapToTarget);
		//UE_LOG(LogTemp, Warning, TEXT("AWeapon_M9::AddWeapon says: Spawn Complete!"));
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Yellow, FString::FString("Spawn Complete"));
	}
}

void AWeapon_M9::ShootWeapon()
{
	/* We don't spawn a blueprint anymore, we will line trace it
	FVector location = LaserSource->GetComponentLocation();
	FRotator rotation = LaserSource->GetComponentRotation();

	GetWorld()->SpawnActor<AActor>(BulletBlueprint, location, rotation);
	*/
	
	if (LaserBeam->IsVisible())
	{
		FHitResult RV_Hit(ForceInit);
		AWeapon_M9::singleLineTrace(RV_Hit);
		const FRotator perperndicularRotation = { 0.0f, 90.0f, 0.0f };

		//gunshot effect
		GetWorld()->SpawnActor<AActor>(Gunshot_InitialEffect, LaserSource->GetComponentLocation(), LaserSource->GetComponentRotation());

		//empty shell
		GetWorld()->SpawnActor<AActor>(Empty_Shell, Mesh->GetComponentLocation() + (0.0f, 0.0f, 4.0f), LaserSource->GetComponentRotation() + perperndicularRotation);

		//on hit!
		if (RV_Hit.bBlockingHit)
		{
			AWeapon_M9::OnHit(RV_Hit);
		}
	}
}

void AWeapon_M9::OnHit(FHitResult hitResult)
{
	//gunshot hit effect
	GetWorld()->SpawnActor<AActor>(Gunshot_FinalEffect, LaserImpact->GetComponentLocation(), LaserImpact->GetComponentRotation());

	ARotatableTarget* rotatableTarget = Cast<ARotatableTarget>(hitResult.GetActor());
	UActorComponent* objectDestruct = hitResult.GetActor()->GetComponentByClass(UDestructibleComponent::StaticClass());

	//did I hit a target?
	if (rotatableTarget)
	{
		UActorComponent* objectDestruct = hitResult.GetActor()->GetComponentByClass(UDestructibleComponent::StaticClass());
		rotatableTarget->OnTargetHit();
	}

	//did I hit a destructible?
	if (objectDestruct)
	{
		UDestructibleComponent* destructibleComponent = Cast<UDestructibleComponent>(objectDestruct);
		destructibleComponent->ApplyDamage(1000.0f, hitResult.Location, FVector(1.0f, 1.0f, 1.0f), 100.0f);
	}
	
}

void AWeapon_M9::UpdateLaserPointer()
{
	// create a pointer on the player character
	ACharacter *player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AMainCharacter* mainChar = Cast<AMainCharacter>(player);

	bool bIsAiming = mainChar->bCameraZoomIn;
	LaserBeam->SetVisibility(bIsAiming);
	LaserImpact->SetVisibility(bIsAiming);
	if (bIsAiming)
	{
		FHitResult RV_Hit(ForceInit);

		AWeapon_M9::singleLineTrace(RV_Hit);
		LaserTargetLocation = RV_Hit.bBlockingHit ? RV_Hit.Location : RV_Hit.TraceEnd;
		LaserBeam->SetBeamSourcePoint(0, RV_Hit.TraceStart, 0);
		LaserBeam->SetBeamTargetPoint(0, LaserTargetLocation, 0);
		LaserImpact->SetWorldLocation(LaserTargetLocation - LaserImpact->GetForwardVector());
	}
}

bool AWeapon_M9::singleLineTrace(FHitResult &RV_Hit)
{
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	FVector Start = LaserSource->GetComponentLocation();
	FVector End = Start + (LaserSource->GetForwardVector() * 5000);

	//call GetWorld() from within an actor extending class
	return GetWorld()->LineTraceSingleByChannel(
		RV_Hit,			//result
		Start,			//start
		End,			//end
		ECC_Visibility, //collision channel
		RV_TraceParams
	);
}