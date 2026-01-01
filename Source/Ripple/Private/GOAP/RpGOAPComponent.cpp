// Copyright Anupam Sahu. All Rights Reserved.

#include "GOAP/RpGOAPComponent.h"

#include "GOAP/RpGOAPAction.h"
#include "GOAP/RpGOAPGoal.h"
#include "GOAP/RpGOAPPlanner.h"
#include "StateMachine/RpStateMachineBlackboard.h"

URpGOAPComponent::URpGOAPComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URpGOAPComponent::BeginPlay()
{
	StateMachineBlackboard = NewObject<URpStateMachineBlackboardBase>(GetTransientPackage(), StatemachineBBClass);
	Planner = NewObject<URpGOAPPlanner>(GetTransientPackage(), PlannerClass);
	for (const auto& GoalClass : GoalClasses)
	{
		Planner->AddGoal(GoalClass);
	}
	for (const auto& ActionClass : ActionClasses)
	{
		URpGOAPAction* NewAction = NewObject<URpGOAPAction>(GetTransientPackage(), ActionClass);
		NewAction->SetContext(StateMachineBlackboard);
		Planner->AddAction(NewAction);
	}
	
	StartingState = NewObject<URpGOAPState>(GetTransientPackage(), StartingStateClass);
	Planner->SetStartingState(StartingState);
	
	Super::BeginPlay();
}

void URpGOAPComponent::OnActionComplete(URpGOAPState* State)
{
	if (URpGOAPGoal* CurrentGoal = Planner->GetCurrentGoal())
	{
		if (State->DoesSatisfyRequirements(CurrentGoal->GetRequirements()))
		{
			OnGoalReached.Broadcast(CurrentGoal);
		}
	}
}

void URpGOAPComponent::CreatePlan()
{
	URpGOAPGoal* ChosenGoal = Planner->PickGoal();
	if (!ChosenGoal)
	{
		return;
	}
	
	Planner->CreatePlan(ChosenGoal, ActionPlan);
	for (URpGOAPAction* Action : ActionPlan)
	{
		Action->OnActionCompleteEvent.AddUniqueDynamic(this, &URpGOAPComponent::OnActionComplete);
	}
}

void URpGOAPComponent::ExecutePlan()
{
	for (int Index = 0; Index < ActionPlan.Num() - 1; ++Index)
	{
		URpGOAPAction* Action = ActionPlan[Index];
		if (ActionPlan.IsValidIndex(Index + 1))
		{
			URpGOAPAction* NextAction = ActionPlan[Index + 1];
			Action->SetNextAction(NextAction);
		}
	}
	
	if (!ActionPlan.IsEmpty())
	{
		ActionPlan[0]->Execute(StartingState);
	}
}