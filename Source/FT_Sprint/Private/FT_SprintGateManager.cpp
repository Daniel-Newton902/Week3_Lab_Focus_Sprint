// Fill out your copyright notice in the Description page of Project Settings.


#include "FT_SprintGateManager.h"

// Sets default values
AFT_SprintGateManager::AFT_SprintGateManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFT_SprintGateManager::BeginPlay()
{
	Super::BeginPlay();
	// check for multiple managers in level
	if(instance == nullptr)
	{
		// we are the one and only
		instance = this;
	}
	else
	{
		// we are a duplicate, destroy ourself
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Multiple Sprint Gate Managers detected in level!"));
	}

	// get all the sprint gates in the level
	for (TActorIterator<AFT_SprintGate> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AFT_SprintGate* gate = *ActorItr;
		sprintGates.Add(gate);
	}
}

// Called every frame
void AFT_SprintGateManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (gatesPassed > 0)
	{
		timePassed += DeltaTime;
		if(timePassed >= gateTimeLimit)
		{
			// time's up, reset the track
			ResetTrack();
		}
	}

}

// method to tell all the sprint gates the track is complete shutdown
void AFT_SprintGateManager::CompleteTrack()
{
	UE_LOG(LogTemp, Warning, TEXT("Track complete! Smashed it!"));
	for (AFT_SprintGate* gate : sprintGates)
	{
		gate->completedTrack = true;
	}
}

// every time a gate is passed they will signal this function
void AFT_SprintGateManager::GatePassed()
{
	UE_LOG(LogTemp, Warning, TEXT("Gate Passed!"));
	// keep track of the gates passed
	gatesPassed++;
	// if all gates are passed then complete the track
	if (gatesPassed >= sprintGates.Num())
	{
		CompleteTrack();
	}
}

// reset all the gates to false, and reset the tracking variables
void AFT_SprintGateManager::ResetTrack()
{
	UE_LOG(LogTemp, Warning, TEXT("Track failed reseting!"));
	// set all the gates to false
	for (AFT_SprintGate* gate : sprintGates)
	{
		gate->isTriggered = false;
	}
	// reset tracking variables
	gatesPassed = 0;
	timePassed = 0.0f;
}

