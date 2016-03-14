// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "MainCharacter.h"
#include "HUDUserWidget.h"


void UHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHUDUserWidget::InitWidget(UWorld* World)
{
	MyCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
}