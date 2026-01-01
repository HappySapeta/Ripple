// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GOAP/RpGOAPState.h"
#include "RpGOAPPlanner.generated.h"

class URpGOAPGoal;
class URpGOAPAction;

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
	
public:
	
	void AddGoal(URpGOAPGoal* NewGoal);
	void AddAction(URpGOAPAction* NewAction);
	void SetStartingState(URpGOAPState* StartingState);
	
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "GoalSubClass"))
	URpGOAPGoal* GetGoalOfType(TSubclassOf<URpGOAPGoal> GoalSubClass);
	
	UFUNCTION(BlueprintCallable)
	URpGOAPGoal* PickGoal();
	
	UFUNCTION(BlueprintCallable)
	void CreatePlan(URpGOAPGoal* ChosenGoal, TArray<URpGOAPAction*>& PrimaryActionPlan);

	UFUNCTION(BlueprintCallable)
	URpGOAPState* Simulate(const URpGOAPState* Input, const URpGOAPAction* Action);
	
	UFUNCTION(BlueprintCallable)
	URpGOAPGoal* GetCurrentGoal()
	{
		return PrimaryGoal;
	}

protected:

	void GetAvailableActionsFor(URpGOAPState* CurrentState, TArray<URpGOAPAction*>& AvailableActions, TArray<URpGOAPAction*>& UnavailableActions);
	void PerformAStar(URpGOAPGoal* CurrentGoal, TArray<URpGOAPAction*>& ActionPlan);

protected:
	
	UPROPERTY(BlueprintReadOnly)
	TArray<URpGOAPGoal*> Goals;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<URpGOAPAction*> Actions;
	
	UPROPERTY(BlueprintReadOnly)
	URpGOAPState* StartingState;
	
	UPROPERTY(BlueprintReadOnly)
	URpGOAPGoal* PrimaryGoal;
	
private:
	
	UPROPERTY()
	TArray<URpGOAPState*> OpenSet;
};
