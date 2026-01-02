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
	BaseState = State;
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
	if (Goals.IsEmpty())
	{
		return nullptr;
	}
	
	URpGOAPGoal* ChosenGoal = Goals.Top();
	return ChosenGoal;
}

void URpGOAPPlanner::CreatePlan(URpGOAPGoal* ChosenGoal, TArray<URpGOAPAction*>& PrimaryActionPlan)
{
	UE_LOG(LogTemp, Warning, TEXT("------PERFORMING ASTAR------"));
	PerformAStar(BaseState, ChosenGoal, PrimaryActionPlan);
	UE_LOG(LogTemp, Warning, TEXT("------ASTAR COMPLETE--------"));
	
	int Index = 0;
	for (const URpGOAPAction* Action : PrimaryActionPlan)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action %d : %s"), ++Index, *Action->GetName());
	}
}

void URpGOAPPlanner::PerformAStar(URpGOAPState* StartingState, URpGOAPGoal* Goal, TArray<URpGOAPAction*>& ActionPlan)
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
	
	static int COUNT;
	COUNT = 10;
	while (OpenSet.Num() > 0 && COUNT-- > 0)
	{
		URpGOAPState* Current;
		OpenSet.HeapPop(Current, FMostOptimalState());
		Current->GetAStarNode().SetSeen(true);
		
		// GOAL HAS BEEN REACHED
		const FString CurrentFactsAsString = *Current->FactsToString();
		UE_LOG(LogTemp, Log, TEXT("GOAL CHECK : CURRENT FACTS \n %s"), *CurrentFactsAsString);
		const FString GoalReqsAsString = Goal->RequirementsToString();
		UE_LOG(LogTemp, Log, TEXT("GOAL CHECK : GOAL FACTS \n %s"), *GoalReqsAsString);
		
		if (Current->DoesSatisfyRequirements(Goal->GetRequirements()))
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
					URpGOAPGoal* IntermediateGoal = NewObject<URpGOAPGoal>(GetOuter());
					IntermediateGoal->SetRequirements(Action->GetRequirements());
				
					TArray<URpGOAPAction*> IntermediateActions;
					PerformAStar(Current, IntermediateGoal, IntermediateActions);
				
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
			URpGOAPState* NewState = DuplicateObject(Current, GetOuter());
			Action->Simulate(NewState);
			
			const FString NewStateFacts = NewState->FactsToString();
			UE_LOG(LogTemp, Log, TEXT("NEWSTATE FACTS \n %s"), *NewStateFacts);
			
			if (!NewState || NewState->GetAStarNode().IsSeen())
			{
				continue;
			}
			
			const int NewGCost = Current->GetAStarNode().GetGCost() + Current->CalcDistanceFromState(NewState) + Action->GetCost();
			
			const bool bIsInOpen = OpenSet.Contains(NewState);
			if (NewGCost < NewState->GetAStarNode().GetGCost() || !bIsInOpen)
			{
				NewState->GetAStarNode().SetGCost(NewGCost);
				NewState->GetAStarNode().SetHCost(NewState->CalcDistanceFromGoal(Goal));
				NewState->GetAStarNode().SetLinkingAction(Action);
				NewState->GetAStarNode().SetParent(Current);
				
				if (!bIsInOpen)
				{
					OpenSet.HeapPush(NewState, FMostOptimalState());
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
	
	URpGOAPGoal* NewGoal = NewObject<URpGOAPGoal>(GetOuter(), GoalSubClass);
	Goals.HeapPush(NewGoal, FMostImportantGoal());
	
	OnGoalsUpdatedEvent.Broadcast();
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
	
	OnGoalsUpdatedEvent.Broadcast();
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
