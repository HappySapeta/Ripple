// Copyright Anupam Sahu. All Rights Reserved.

#include "GOAP/RpGOAPComponent.h"

#include "GOAP/RpGOAPAction.h"
#include "GOAP/RpGOAPPlanner.h"

URpGOAPComponent::URpGOAPComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Planner = NewObject<URpGOAPPlanner>();
	for (const auto& GoalClass : GoalClasses)
	{
		Planner->AddGoal(NewObject<URpGOAPGoal>(GetTransientPackage(), GoalClass));
	}
	for (const auto& ActionClass : ActionClasses)
	{
		Planner->AddAction(NewObject<URpGOAPAction>(GetTransientPackage(), ActionClass));
	}
	Planner->SetStartingState(NewObject<URpGOAPState>(GetTransientPackage(), StartingStateClass));
}

URpGOAPPlanner* URpGOAPComponent::GetPlanner()
{
	return Planner;
}

void URpGOAPComponent::BeginPlay()
{
	Super::BeginPlay();
	
	const URpGOAPGoal* ChosenGoal = Planner->PickGoal();
	Planner->CreatePlan(ChosenGoal);
}



