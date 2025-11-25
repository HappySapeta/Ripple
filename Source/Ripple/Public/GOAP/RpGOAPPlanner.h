// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RpGOAPGoal.h"
#include "RpGOAPPlanner.generated.h"

class URpGOAPAction;
class URpGOAPState;

/** 
 * 
 */
UCLASS(Blueprintable, BlueprintType, Category = "Ripple GOAP")
class RIPPLE_API URpGOAPPlanner : public UObject
{
	GENERATED_BODY()
	
public:
	
	const URpGOAPGoal* PickGoal();
	void CreatePlan(const URpGOAPGoal* ChosenGoal);
	
	UFUNCTION(BlueprintCallable)
	const URpGOAPState* Simulate(const URpGOAPState* Input, const URpGOAPAction* Action);
	
protected:
	
	bool AreRequirementsSatisfied(const TMap<FGameplayTag, FRpRequirementDescriptor>& Requirements, const URpGOAPState* State) const;
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TArray<URpGOAPGoal*> Goals;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<URpGOAPAction>> Actions; 
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URpGOAPState> StartingState;
	
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<URpGOAPState> CurrentState;
	
private:
	
	UPROPERTY()
	const URpGOAPGoal* CurrentGoal;
};
