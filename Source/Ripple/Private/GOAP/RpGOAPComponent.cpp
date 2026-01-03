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
	// Initialize the blackboard/context.
	{
		GOAPBlackboard = NewObject<URpStateMachineBlackboardBase>(this, StatemachineBlackboardClass);
	}
	
	// Initialize the GOAP planner.
	{
		Planner = NewObject<URpGOAPPlanner>(this, PlannerClass);
		for (const auto& ActionClass : ActionClasses)
		{
			URpGOAPAction* NewAction = NewObject<URpGOAPAction>(this, ActionClass);
			NewAction->SetContext(GOAPBlackboard);
			Planner->AddAction(NewAction);
		}
	}
	
	// Initialize the state of the Agent.
	{
		BaseState = NewObject<URpGOAPState>(this, StartingStateClass);
	}
	
	// Call blueprint BeginPlay.
	Super::BeginPlay();
}

URpGOAPGoal* URpGOAPComponent::GetGoalOfType(TSubclassOf<URpGOAPGoal> GoalSubClass)
{
	for (URpGOAPGoal* Goal : Goals)
	{
		if (Goal->IsA(GoalSubClass))
		{
			return Goal;
		}
	}
	return nullptr;
}

URpGOAPGoal* URpGOAPComponent::PickGoal()
{
	if (Goals.IsEmpty())
	{
		return nullptr;
	}
	
	// Chose the goal with the highest priority.
	CurrentGoal = Goals.Top();
	
	return CurrentGoal;
}

void URpGOAPComponent::PlanAndExecute()
{
	TArray<URpGOAPAction*> ActionPlan;
	Planner->CreatePlan(BaseState, CurrentGoal, ActionPlan);
	
	// Link actions in a chain.
	{
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
	}
	
	// Execute first action in the chain.
	if (!ActionPlan.IsEmpty())
	{
		ActionPlan[0]->Execute(BaseState);
	}
}

URpGOAPGoal* URpGOAPComponent::AddGoal(TSubclassOf<URpGOAPGoal> GoalSubClass)
{
	// Check if a goal of the input type already exists and return it if it does.
	{
		if (URpGOAPGoal** FoundGoal = Goals.FindByPredicate([GoalSubClass](const URpGOAPGoal* TargetGoal)
	   {
		   return TargetGoal->IsA(GoalSubClass);
	   }))
		{
			if (FoundGoal)
			{
				return *FoundGoal;
			}
		}
	}
	
	// Create new goal.
	URpGOAPGoal* NewGoal = NewObject<URpGOAPGoal>(GetOuter(), GoalSubClass);
	Goals.HeapPush(NewGoal, FMostImportantGoal());
	
	return NewGoal;
}

void URpGOAPComponent::RemoveGoal(TSubclassOf<URpGOAPGoal> GoalSubClass)
{
	for (URpGOAPGoal* Goal : Goals)
	{
		if (Goal->IsA(GoalSubClass))
		{
			Goals.HeapPop(Goal, FMostImportantGoal());
			break;
		}
	}
}

void URpGOAPComponent::OnActionComplete(URpGOAPState* State)
{
	if (State->DoesSatisfyRequirements(CurrentGoal->GetRequirements()))
	{
		OnGoalReached.Broadcast(CurrentGoal);
	}
}