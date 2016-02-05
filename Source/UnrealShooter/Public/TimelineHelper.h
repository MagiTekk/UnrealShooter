// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 *	Class: the class that calls this helper
	onUpdateFuntionName : the name of the function that will get called on update
	WorldTimeManager: a reference to the worldTimeManager
 */
class UNREALSHOOTER_API TimelineHelper : public AActor
{
public:
	
	TimelineHelper(AActor* ObjInstance, FName onUpdateFuntionName);
	~TimelineHelper();

	/** Timeline for the effectprogress*/
	FTimeline* TimeLine;

	/** Need a handle to manage the timeline functions */
	FTimerHandle TimerHandle;

	/** Deltatime(stepsize) for the timer calling the timeline tick */
	const float DELTATIME = 1.0f;

	/** Function which gets called from the Timer to call TimelineProgress */
	void TickTimeline();

	/** Function which gets called from the Timeline on a Tick */
	UFUNCTION()
	void TimelineProgress(float Value, void(*externalFunction)(float) );

	/** Basically the "BeginPlay" function, since this is not an Actor I called it start */
	void start();
	
};
