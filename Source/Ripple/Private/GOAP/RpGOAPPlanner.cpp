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
	
	
	
	//int Result = PerformDFS(StartingState, ActionPlan);
	//if (Result == TNumericLimits<int>::Max())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("No path could be found!"));
	//}
	//else if (Result == 0)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Already at goal."));
	//}
	//else
	//{
	//	int Step = 0;
	//	for (const URpGOAPAction* Action : ActionPlan)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("%d Action : %s"), ++Step, *Action->GetName());
	//	}
	//}
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
	
	StartingState->SetGCost(0);
	// TODO : URpGOAPGoal::CalcDistanceFromState() implementation pending
	StartingState->SetHCost(StartingState->CalcDistanceFromGoal(Goal));
	
	OpenSet.Push(StartingState);
	
	while (!OpenSet.IsEmpty())
	{
		URpGOAPState* Current;
		OpenSet.HeapPop(Current, FMostOptimalState());
		Current->SetSeen(true);
		
		// GOAL HAS BEEN REACHED
		if (Current->DoesSatisfyRequirements(Goal->GetRequirements()))
		{
			GoalState = Current;
		}
		
		for (URpGOAPAction* Action : GetAvailableActionsFor(Current))
		{
			URpGOAPState* Neighbor = Simulate(Current, Action);
			if (Neighbor->IsSeen()) // TODO : Avoid comparing pointers
			{
				continue;
			}
			
			// TODO : URpGOAPState::DistanceFrom() Implementation pending
			const int NewGCost = Current->GetGCost() + Current->CalcDistanceFromState(Neighbor);
			const bool bIsInOpen = OpenSet.Contains(Neighbor); // TODO : Avoid comparing pointers
			if (NewGCost < Neighbor->GetGCost() || !bIsInOpen)
			{
				Neighbor->SetGCost(NewGCost);
				Neighbor->SetHCost(Neighbor->CalcDistanceFromGoal(Goal));
				Neighbor->SetLinkingAction(Action);
				
				if (!bIsInOpen)
				{
					OpenSet.HeapPush(Neighbor, FMostOptimalState());
				}
			}
		}
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
		StateCopy->SetFact(FactName, EffectDescriptor);
	}
	
	return StateCopy;
}
