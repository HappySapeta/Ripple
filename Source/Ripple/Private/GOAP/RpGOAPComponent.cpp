// Copyright Anupam Sahu. All Rights Reserved.

#include "GOAP/RpGOAPComponent.h"

#include "GOAP/RpGOAPAction.h"
#include "GOAP/RpGOAPGoal.h"
#include "GOAP/RpGOAPPlanner.h"

URpGOAPComponent::URpGOAPComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

URpGOAPPlanner* URpGOAPComponent::GetPlanner()
{
	return Planner;
}

void URpGOAPComponent::InitializePlanner()
{
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

void URpGOAPComponent::CreatePlan()
{
	URpGOAPGoal* ChosenGoal = Planner->PickGoal();
	Planner->CreatePlan(ChosenGoal);
}




