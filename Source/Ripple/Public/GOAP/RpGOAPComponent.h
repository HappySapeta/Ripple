// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RpGOAPComponent.generated.h"

class URpGOAPState;
class URpGOAPGoal;
class URpGOAPAction;
class URpGOAPPlanner;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RIPPLE_API URpGOAPComponent : public UActorComponent
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalReachedDelegate, URpGOAPGoal*, Goal);

public:

	// Sets default values for this component's properties
	URpGOAPComponent();
	
	UFUNCTION(BlueprintCallable)
	URpGOAPPlanner* GetPlanner()
	{
		return Planner;
	}
	
	UFUNCTION(BlueprintCallable)
	void CreatePlan();
	
	UFUNCTION(BlueprintCallable)
	void ExecutePlan();
	
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ContextSubClass"))
	URpStateMachineBlackboardBase* GetContext(TSubclassOf<URpStateMachineBlackboardBase> ContextSubClass)
	{
		return StateMachineBlackboard;
	}

	virtual void BeginPlay() override;
	
private:
	
	UFUNCTION()
	void OnActionComplete(URpGOAPState* State);
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnGoalReachedDelegate OnGoalReached;

protected:
	
	UPROPERTY(BlueprintReadOnly)
	URpGOAPPlanner* Planner;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<URpStateMachineBlackboardBase> StateMachineBlackboard;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Ripple GOAP")
	TSubclassOf<URpGOAPPlanner> PlannerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ripple GOAP")
	TSubclassOf<URpGOAPState> StartingStateClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ripple GOAP")
	TArray<TSubclassOf<URpGOAPGoal>> GoalClasses;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ripple GOAP")
	TArray<TSubclassOf<URpGOAPAction>> ActionClasses;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ripple GOAP")
	TSubclassOf<URpStateMachineBlackboardBase> StatemachineBBClass;
	
	UPROPERTY()
	TArray<URpGOAPAction*> ActionPlan;
	
	UPROPERTY()
	URpGOAPState* StartingState;
};
