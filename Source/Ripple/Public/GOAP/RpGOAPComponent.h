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
	
	virtual void BeginPlay() override;
	
public:

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ContextSubClass"))
	URpStateMachineBlackboardBase* GetContext(TSubclassOf<URpStateMachineBlackboardBase> ContextSubClass)
	{
		return GOAPBlackboard;
	}
	
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "GoalSubClass"))
	URpGOAPGoal* GetGoalOfType(TSubclassOf<URpGOAPGoal> GoalSubClass);
	
	UFUNCTION(BlueprintCallable)
	URpGOAPState* GetBaseState()
	{
		return BaseState;
	}
	
public:

	UFUNCTION(BlueprintCallable)
	URpGOAPGoal* PickGoal();
	
	UFUNCTION(BlueprintCallable)
	void PlanAndExecute();
	
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "GoalSubClass"))
	URpGOAPGoal* AddGoal(TSubclassOf<URpGOAPGoal> GoalSubClass);
	
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "GoalSubClass"))
	void RemoveGoal(TSubclassOf<URpGOAPGoal> GoalSubClass);
	
private:
	
	UFUNCTION()
	void OnActionComplete(URpGOAPState* State);
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnGoalReachedDelegate OnGoalReached;

private:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URpGOAPPlanner> PlannerClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URpGOAPState> StartingStateClass;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<URpGOAPAction>> ActionClasses;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URpStateMachineBlackboardBase> StatemachineBlackboardClass;
	
	UPROPERTY()
    URpGOAPPlanner* Planner;
    
    UPROPERTY()
    TObjectPtr<URpStateMachineBlackboardBase> GOAPBlackboard;
	
	UPROPERTY()
	URpGOAPState* BaseState;
	
	UPROPERTY()
	URpGOAPGoal* CurrentGoal;
	
	UPROPERTY()
	TArray<URpGOAPGoal*> Goals;
};
