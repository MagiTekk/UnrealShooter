// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealShooter.h"
#include "TimelineHelper.h"



TimelineHelper::TimelineHelper(AActor* ObjInstance, FName onUpdateFuntionName)
{
	const ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("CurveFloat'/UnrealShooter/Animation/CurveFloat/TimeLineHelperCurve.TimeLineHelperCurve'"));
	
	TimeLine = new FTimeline();
	FOnTimelineFloat progressFunction{};
	progressFunction.BindUFunction(ObjInstance, onUpdateFuntionName); // The function TimelineProgress calls
	TimeLine->AddInterpFloat(Curve.Object, progressFunction, FName{ TEXT("EFFECTFADE") });
}

TimelineHelper::~TimelineHelper()
{
}

void TimelineHelper::TimelineProgress(float Value, void(*externalFunction)(float))
{
	(*externalFunction)(Value);
}

void TimelineHelper::start()
{
	TimeLine->PlayFromStart();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &TimelineHelper::TickTimeline, DELTATIME, true, 0.0f);
}

/** Function which gets called from the Timer to call EffectProgress */
void TimelineHelper::TickTimeline()
{
	if (TimeLine->IsPlaying())
	{
		TimeLine->TickTimeline(DELTATIME);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}