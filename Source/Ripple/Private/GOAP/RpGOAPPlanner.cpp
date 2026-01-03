// Copyright Anupam Sahu. All Rights Reserved.

#include "GOAP/RpGOAPPlanner.h"
#include "GOAP/RpGOAPState.h"
#include "GOAP/RpGOAPAction.h"
#include "GOAP/RpGOAPGoal.h"

constexpr int MAX_ASTAR_TRIALS = 10;

void URpGOAPPlanner::AddAction(URpGOAPAction* NewAction)
{
	Actions.Push(NewAction);
}

void URpGOAPPlanner::CreatePlan(URpGOAPState* StartingState, URpGOAPGoal* Goal, TArray<URpGOAPAction*>& Out_ActionPlan)
{
	UE_LOG(LogTemp, Warning, TEXT("------ PERFORMING ASTAR ------"));
	PerformAStar(StartingState, Goal, Out_ActionPlan);
	UE_LOG(LogTemp, Warning, TEXT("------ ASTAR COMPLETE --------"));
	
	int Index = 0;
	for (const URpGOAPAction* Action : Out_ActionPlan)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action %d : %s"), ++Index, *Action->GetName());
	}
}

void URpGOAPPlanner::PerformAStar(URpGOAPState* StartingState, URpGOAPGoal* Goal, TArray<URpGOAPAction*>& Out_ActionPlan)
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
	
	int Trials = 0;
	while (OpenSet.Num() > 0)
	{
		if (++Trials > MAX_ASTAR_TRIALS)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find path to goal."));
			break;
		}
		
		URpGOAPState* Current;
		OpenSet.HeapPop(Current, FMostOptimalState());
		Current->GetAStarNode().SetSeen(true);
		
		// Check if goal has been reached.
		UE_LOG(LogTemp, Warning, TEXT("CHECKING IF GOAL HAS BEEN REACHED"));
		Current->PrintFacts("Current");
		Goal->PrintRequirements("Goal");
		if (Current->DoesSatisfyRequirements(Goal->GetRequirements()))
		{
			GoalState = Current;
			break;
		}
		
		TArray<URpGOAPAction*> AvailableActions;
		{
			TArray<URpGOAPAction*> UnavailableActions;
			GetActions(Current, AvailableActions, UnavailableActions);
		
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
						Out_ActionPlan.Append(IntermediateActions);
						AvailableActions.Push(Action);
					}
				}
			}
		}
		
		for (URpGOAPAction* Action : AvailableActions)
		{
			URpGOAPState* NewState = DuplicateObject(Current, GetOuter());
			Action->Simulate(NewState);
			NewState->PrintFacts("NewState");
			
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
	
	GeneratePath(GoalState, Out_ActionPlan);
}

void URpGOAPPlanner::GeneratePath(URpGOAPState* GoalState, TArray<URpGOAPAction*>& Out_ActionPlan)
{
	Out_ActionPlan.Reset();
	URpGOAPState* Current = GoalState;
	while (Current)
	{
		URpGOAPAction* Action = Current->GetAStarNode().GetLinkingAction();
		if (!Action)
		{
			break;
		}
		Out_ActionPlan.Push(Action);
		Current = Current->GetAStarNode().GetParent();
	}
	Algo::Reverse(Out_ActionPlan);
}

void URpGOAPPlanner::GetActions
(
	URpGOAPState* ForState, 
	TArray<URpGOAPAction*>& Out_Available, 
	TArray<URpGOAPAction*>& Out_Unavailable
)
{
	for (const auto& Action : Actions)
	{
		if (ForState->DoesSatisfyRequirements(Action->GetRequirements()) && 
			ForState->WillHaveEffects(Action->GetEffects()))
		{
			Out_Available.Push(Action);
		}
		else
		{
			Out_Unavailable.Push(Action);
		}
	}
}
