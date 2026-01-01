// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "RpGOAPTypes.h"
#include "RpGOAPAction.generated.h"

class URpGOAPState;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class RIPPLE_API URpGOAPAction : public UObject
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionCompleteDelgate, URpGOAPState*, State);
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SetContext(URpStateMachineBlackboardBase* NewBlackboard)
	{
		Blackboard = NewBlackboard;
	}
	
	// Return State Blackboard as a subtype of IRpStateContext.
	// For use in blueprints only.
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ContextSubClass"))
	URpStateMachineBlackboardBase* GetContext(TSubclassOf<URpStateMachineBlackboardBase> ContextSubClass) const
	{
		return Blackboard;
	}
	
	void SetNextAction(URpGOAPAction* NextAction = nullptr)
	{
		NextActionToRun = NextAction;
	}
	
	virtual void Execute(URpGOAPState* State);
	
	float GetCost() const
	{
		return Cost;	
	}
	
	void SetCost(const float NewCost)
	{
		Cost = NewCost;
	}
	
	const TMap<FGameplayTag, FRpRequirementDescriptor>& GetRequirements() const
	{
		return Requirements;
	}
	
	const FFactsContainer& GetEffects() const
	{
		return Effects;
	}
	
public:
	
	UPROPERTY(BlueprintReadOnly)
	FOnActionCompleteDelgate OnActionCompleteEvent;
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnPerformAction")
	void BP_OnPerformAction();
	
	UFUNCTION(BlueprintCallable)
	void OnActionComplete();

protected:
	
	UPROPERTY(EditAnywhere)
	float Cost = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FRpRequirementDescriptor> Requirements;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FRpStateDescriptor> Effects;
	
	UPROPERTY(BlueprintReadOnly)
	URpGOAPState* TargetState;
	
	UPROPERTY()
	URpGOAPAction* NextActionToRun;
	
private:
	
	// The Blackboard object of this state that contains all the information that it needs.
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URpStateMachineBlackboardBase> Blackboard;
};
