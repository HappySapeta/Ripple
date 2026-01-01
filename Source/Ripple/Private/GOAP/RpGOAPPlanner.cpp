// Copyright Anupam Sahu. All Rights Reserved.

#include "GOAP/RpGOAPPlanner.h"
#include "GOAP/RpGOAPState.h"
#include "GOAP/RpGOAPAction.h"
#include "GOAP/RpGOAPGoal.h"

void URpGOAPPlanner::AddAction(URpGOAPAction* NewAction)
{
	Actions.Push(NewAction);
}

void URpGOAPPlanner::SetStartingState(URpGOAPState* State)
{
	StartingState = State;
}

URpGOAPGoal* URpGOAPPlanner::GetGoalOfType(TSubclassOf<URpGOAPGoal> GoalSubClass)
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

// O(n) : n = number of goals.
URpGOAPGoal* URpGOAPPlanner::PickGoal()
{
	URpGOAPGoal* ChosenGoal = Goals.Top();
	return ChosenGoal;
}

void URpGOAPPlanner::CreatePlan(URpGOAPGoal* ChosenGoal, TArray<URpGOAPAction*>& PrimaryActionPlan)
{
	PrimaryGoal = ChosenGoal;
	PerformAStar(PrimaryGoal, PrimaryActionPlan);
	
	int Index = 0;
	for (const URpGOAPAction* Action : PrimaryActionPlan)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action %d : %s"), ++Index, *Action->GetName());
	}
}

void URpGOAPPlanner::PerformAStar(URpGOAPGoal* CurrentGoal, TArray<URpGOAPAction*>& ActionPlan)
{
	URpGOAPState* GoalState = nullptr;
	if(!StartingState || !PrimaryGoal)
	{
		return;
	}
	
	StartingState->GetAStarNode().SetGCost(0);
	StartingState->GetAStarNode().SetHCost(StartingState->CalcDistanceFromGoal(PrimaryGoal));
	StartingState->GetAStarNode().SetLinkingAction(nullptr);
	
	OpenSet.Push(StartingState);
	
	while (OpenSet.Num() > 0)
	{
		URpGOAPState* Current;
		OpenSet.HeapPop(Current, FMostOptimalState());
		Current->GetAStarNode().SetSeen(true);
		
		// GOAL HAS BEEN REACHED
		if (Current->DoesSatisfyRequirements(PrimaryGoal->GetRequirements()))
		{
			GoalState = Current;
			break;
		}
		
		TArray<URpGOAPAction*> AvailableActions;
		{
			TArray<URpGOAPAction*> UnavailableActions;
			GetAvailableActionsFor(Current, AvailableActions, UnavailableActions);
		
			if (AvailableActions.IsEmpty() && !UnavailableActions.IsEmpty())
			{
				// set new goals to fullfill requirements and make actions available.			
				for (URpGOAPAction* Action : UnavailableActions)
				{
					URpGOAPGoal* IntermediateGoal = NewObject<URpGOAPGoal>(GetTransientPackage());
					IntermediateGoal->SetRequirements(Action->GetRequirements());
				
					TArray<URpGOAPAction*> IntermediateActions;
					PerformAStar(IntermediateGoal, IntermediateActions);
				
					if (!IntermediateActions.IsEmpty())
					{
						ActionPlan.Append(IntermediateActions);
						AvailableActions.Push(Action);
					}
				}
			}
		}
		
		for (URpGOAPAction* Action : AvailableActions)
		{
			URpGOAPState* Neighbor = Simulate(Current, Action);
			if (!Neighbor || Neighbor->GetAStarNode().IsSeen())
			{
				continue;
			}
			
			const int NewGCost = Current->GetAStarNode().GetGCost() + Current->CalcDistanceFromState(Neighbor) + Action->GetCost();
			
			const bool bIsInOpen = OpenSet.Contains(Neighbor);
			if (NewGCost < Neighbor->GetAStarNode().GetGCost() || !bIsInOpen)
			{
				Neighbor->GetAStarNode().SetGCost(NewGCost);
				Neighbor->GetAStarNode().SetHCost(Neighbor->CalcDistanceFromGoal(PrimaryGoal));
				Neighbor->GetAStarNode().SetLinkingAction(Action);
				Neighbor->GetAStarNode().SetParent(Current);
				
				if (!bIsInOpen)
				{
					OpenSet.HeapPush(Neighbor, FMostOptimalState());
				}
			}
		}
	}
	
	// Prepare path
	{
		ActionPlan.Reset();
		URpGOAPState* Current = GoalState;
		while (Current)
		{
			URpGOAPAction* Action = Current->GetAStarNode().GetLinkingAction();
			if (!Action)
			{
				break;
			}
			ActionPlan.Push(Action);
			Current = Current->GetAStarNode().GetParent();
		}
		Algo::Reverse(ActionPlan);
	}
}

URpGOAPGoal* URpGOAPPlanner::AddGoal(TSubclassOf<URpGOAPGoal> GoalSubClass)
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
	
	URpGOAPGoal* NewGoal = NewObject<URpGOAPGoal>(GetTransientPackage(), GoalSubClass);
	Goals.HeapPush(NewGoal, FMostImportantGoal());
	return NewGoal;
}

void URpGOAPPlanner::RemoveGoal(TSubclassOf<URpGOAPGoal> GoalSubClass)
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

void URpGOAPPlanner::GetAvailableActionsFor
(
	URpGOAPState* CurrentState, 
	TArray<URpGOAPAction*>& AvailableActions, 
	TArray<URpGOAPAction*>& UnavailableActions
)
{
	for (const auto& Action : Actions)
	{
		if (CurrentState->DoesSatisfyRequirements(Action->GetRequirements()) && 
			CurrentState->WillHaveEffects(Action->GetEffects()))
		{
			AvailableActions.Push(Action);
		}
		else
		{
			UnavailableActions.Push(Action);
		}
	}
}

URpGOAPState* URpGOAPPlanner::Simulate(const URpGOAPState* Input, const URpGOAPAction* Action)
{
	URpGOAPState* StateCopy = DuplicateObject(Input, GetOuter());
	for (const auto& [FactName, EffectDescriptor] : Action->GetEffects())
	{
		if (!StateCopy->SetFact(FactName, EffectDescriptor))
		{
			return nullptr;
		}
	}
	
	return StateCopy;
}
