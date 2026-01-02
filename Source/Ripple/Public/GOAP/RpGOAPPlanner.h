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

/** 
 * 
 */
UCLASS(Blueprintable, Category = "Ripple GOAP")
class RIPPLE_API URpGOAPPlanner : public UObject
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoalsUpdatedDelegate);
	
public:
	
	void AddAction(URpGOAPAction* NewAction);
	void SetStartingState(URpGOAPState* StartingState);
	
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "GoalSubClass"))
	URpGOAPGoal* AddGoal(TSubclassOf<URpGOAPGoal> GoalSubClass);
	
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "GoalSubClass"))
	void RemoveGoal(TSubclassOf<URpGOAPGoal> GoalSubClass);

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "GoalSubClass"))
	URpGOAPGoal* GetGoalOfType(TSubclassOf<URpGOAPGoal> GoalSubClass);
	
	UFUNCTION(BlueprintCallable)
	URpGOAPGoal* PickGoal();
	
	UFUNCTION(BlueprintCallable)
	void CreatePlan(URpGOAPGoal* ChosenGoal, TArray<URpGOAPAction*>& PrimaryActionPlan);

protected:

	void GetAvailableActionsFor(URpGOAPState* CurrentState, TArray<URpGOAPAction*>& AvailableActions, TArray<URpGOAPAction*>& UnavailableActions);
	void PerformAStar(URpGOAPState* StartingState, URpGOAPGoal* Goal, TArray<URpGOAPAction*>& ActionPlan);

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnGoalsUpdatedDelegate OnGoalsUpdatedEvent;
	
protected:
	
	UPROPERTY(BlueprintReadOnly)
	TArray<URpGOAPGoal*> Goals;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<URpGOAPAction*> Actions;
	
	UPROPERTY(BlueprintReadOnly)
	URpGOAPState* BaseState;
	
private:
	
	UPROPERTY()
	TArray<URpGOAPState*> OpenSet;
};
