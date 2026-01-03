// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RpGOAPGoal.h"
#include "RpGOAPState.h"
#include "RpGOAPPlanner.generated.h"

class URpGOAPGoal;
class URpGOAPAction;

struct FMostImportantGoal
{
	bool operator()(URpGOAPGoal& A, URpGOAPGoal& B) const
	{
		return A.GetPriority() < B.GetPriority();
	}
};

struct FMostOptimalState
{
	bool operator()(URpGOAPState& A, URpGOAPState& B) const
	{
		return 
		A.GetAStarNode().GetFCost() == B.GetAStarNode().GetFCost() ? 
		A.GetAStarNode().GetHCost() < B.GetAStarNode().GetHCost() : 
		A.GetAStarNode().GetFCost() < B.GetAStarNode().GetFCost(); 
	}
};

UCLASS(Blueprintable, Category = "Ripple GOAP")
class RIPPLE_API URpGOAPPlanner : public UObject
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoalsUpdatedDelegate);
	
public:
	
	void AddAction(URpGOAPAction* NewAction);
	
	UFUNCTION(BlueprintCallable)
	void CreatePlan(URpGOAPState* StartingState, URpGOAPGoal* Goal, TArray<URpGOAPAction*>& Out_ActionPlan);

private:

	void GetActions(URpGOAPState* ForState, TArray<URpGOAPAction*>& Out_Available, TArray<URpGOAPAction*>& Out_Unavailable);
	void PerformAStar(URpGOAPState* StartingState, URpGOAPGoal* Goal, TArray<URpGOAPAction*>& Out_ActionPlan);
	void GeneratePath(URpGOAPState* GoalState, TArray<URpGOAPAction*>& Out_ActionPlan);

private:
	
	UPROPERTY()
	TArray<URpGOAPAction*> Actions;
	
	UPROPERTY()
	TArray<URpGOAPState*> OpenSet;
};
