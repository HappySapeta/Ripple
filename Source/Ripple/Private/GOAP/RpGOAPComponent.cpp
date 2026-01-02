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

void URpGOAPComponent::PlanAndExecute()
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

void URpGOAPComponent::BeginPlay()
{
	StateMachineBlackboard = NewObject<URpStateMachineBlackboardBase>(GetOuter(), StatemachineBBClass);
	Planner = NewObject<URpGOAPPlanner>(this, PlannerClass);

	for (const auto& ActionClass : ActionClasses)
	{
		URpGOAPAction* NewAction = NewObject<URpGOAPAction>(this, ActionClass);
		NewAction->SetContext(StateMachineBlackboard);
		Planner->AddAction(NewAction);
	}
	
	StartingState = NewObject<URpGOAPState>(this, StartingStateClass);
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