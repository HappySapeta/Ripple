// Copyright Anupam Sahu. All Rights Reserved.

#include "GOAP/RpGOAPPlanner.h"
#include "GOAP/RpGOAPState.h"
#include "GOAP/RpGOAPAction.h"
#include "GOAP/RpGOAPGoal.h"

void URpGOAPPlanner::AddGoal(URpGOAPGoal* NewGoal)
{
	Goals.Push(NewGoal);
}

void URpGOAPPlanner::AddAction(URpGOAPAction* NewAction)
{
	Actions.Push(NewAction);
}

void URpGOAPPlanner::SetStartingState(URpGOAPState* State)
{
	StartingState = State;
}

// O(n) : n = number of goals.
const URpGOAPGoal* URpGOAPPlanner::PickGoal()
{
	const URpGOAPGoal* ChosenGoal = nullptr;
	for (const URpGOAPGoal* PotentialGoal : Goals)
	{
		if (!IsValid(ChosenGoal))
		{
			ChosenGoal = PotentialGoal;
			continue;
		}
		
		// Ignore goal if it has currently been achieved.
		if (StartingState->DoesSatisfyRequirements(PotentialGoal->GetRequirements()))
		{
			continue;
		}
		
		if (ChosenGoal->GetPriority() < PotentialGoal->GetPriority())
		{
			ChosenGoal = PotentialGoal;			
		}
		else if (ChosenGoal->GetPriority() == PotentialGoal->GetPriority())
		{
			if (ChosenGoal->GetRequirements().Num() < PotentialGoal->GetRequirements().Num())
			{
				ChosenGoal = PotentialGoal;
			}
		}
	}
	
	return ChosenGoal;	
}

// TODO : Implementation incomplete
void URpGOAPPlanner::CreatePlan(const URpGOAPGoal* ChosenGoal)
{
	Goal = ChosenGoal;
	TArray<const URpGOAPAction*> ActionPlan;
	PerformAStar(ActionPlan);
	
	int Index = 0;
	for (const URpGOAPAction* Action : ActionPlan)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action %d : %s"), ++Index, *Action->GetName());
	}
}

int URpGOAPPlanner::PerformDFS(const URpGOAPState* Current, TArray<const URpGOAPAction*>& ActionPlan)
{
	if (!Current)
	{
		return TNumericLimits<int>::Max();
	}
	
	if (Current->DoesSatisfyRequirements(Goal->GetRequirements()))
	{
		return 0;
	}
	
	int Min = TNumericLimits<int>::Max();
	for (const URpGOAPAction* Action : GetAvailableActionsFor(StartingState))
	{
		const URpGOAPState* NextState = Simulate(StartingState, Action);
		int Cost = Action->GetCost() + PerformDFS(NextState, ActionPlan);
		if (Cost < Min)
		{
			Min = Cost;
			ActionPlan.Push(Action);
		}
	}
	
	return Min;
}

void URpGOAPPlanner::PerformAStar(TArray<const URpGOAPAction*>& ActionPlan)
{
	URpGOAPState* GoalState = nullptr;
	if(!StartingState || !Goal)
	{
		return;
	}
	
	StartingState->GetAStarNode().SetGCost(0);
	StartingState->GetAStarNode().SetHCost(StartingState->CalcDistanceFromGoal(Goal));
	StartingState->GetAStarNode().SetLinkingAction(nullptr);
	
	OpenSet.Push(StartingState);
	
	while (OpenSet.Num() > 0)
	{
		URpGOAPState* Current;
		OpenSet.HeapPop(Current, FMostOptimalState());
		Current->GetAStarNode().SetSeen(true);
		
		// GOAL HAS BEEN REACHED
		if (Current->DoesSatisfyRequirements(Goal->GetRequirements()))
		{
			GoalState = Current;
			break;
		}
		
		for (URpGOAPAction* Action : GetAvailableActionsFor(Current))
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
				Neighbor->GetAStarNode().SetHCost(Neighbor->CalcDistanceFromGoal(Goal));
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
			const URpGOAPAction* Action = Current->GetAStarNode().GetLinkingAction();
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

TArray<URpGOAPAction*> URpGOAPPlanner::GetAvailableActionsFor(URpGOAPState* CurrentState)
{
	TArray<URpGOAPAction*> AvailableActions;
	for (const auto& Action : Actions)
	{
		if (CurrentState->DoesSatisfyRequirements(Action->GetRequirements()))
		{
			AvailableActions.Push(Action);
		}
	}
	
	return AvailableActions;
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
