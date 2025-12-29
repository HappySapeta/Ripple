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
	
	UFUNCTION(BlueprintCallable)
	const URpGOAPGoal* PickGoal();
	
	UFUNCTION(BlueprintCallable)
	void CreatePlan(const URpGOAPGoal* ChosenGoal);

	UFUNCTION(BlueprintCallable)
	URpGOAPState* Simulate(const URpGOAPState* Input, const URpGOAPAction* Action);
	
protected:
	
	TArray<URpGOAPAction*> GetAvailableActionsFor(URpGOAPState* CurrentState);
	int PerformDFS(const URpGOAPState* Current, TArray<const URpGOAPAction*>& ActionPlan);
	void PerformAStar(TArray<const URpGOAPAction*>& ActionPlan);

private:
	
	UPROPERTY()
	TArray<URpGOAPGoal*> Goals;
	
	UPROPERTY()
	TArray<URpGOAPAction*> Actions; 
	
	UPROPERTY()
	URpGOAPState* StartingState;
	
	UPROPERTY()
	const URpGOAPGoal* Goal;
	
private:
	
	UPROPERTY()
	TArray<URpGOAPState*> OpenSet;
};
