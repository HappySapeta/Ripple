// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RpState.generated.h"

// Forward declarations
class URpStateMachineBlackboardBase;

/**
 * Ripple Simple State Machine.
 * UState represents a state of a state machine.
 * All data is received from a State Blackboard object.
 * Derived states provide implementation logic
 * for state Initialization and Execution.
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class RIPPLE_API URpState : public UObject
{
	
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, DisplayName = "OnStateUpdate")
	void StateUpdate(const float DeltaTime);
	virtual void StateUpdate_Implementation(const float DeltaTime) {}
	
	UFUNCTION(BlueprintCallable)
	void SetBlackboard(URpStateMachineBlackboardBase* NewBlackboard)
	{
		Blackboard = NewBlackboard;
	}
	
	void Activate()
	{
		OnActivate();
	}
	
	void DeActivate()
	{
		OnDeactivate();
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetStateName() const
	{
		return StateName;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasFinshed() const
	{
		return bHasFinished;
	}
	
	UFUNCTION(BlueprintCallable)
	void StartState()
	{
		bHasFinished = false;
	}
	
	UFUNCTION(BlueprintCallable)
	void EndState()
	{
		bHasFinished = true;
	}

protected:

	// Return State Blackboard as a subtype of IRpStateContext.
	// For use in blueprints only.
	UFUNCTION(BlueprintCallable)
	URpStateMachineBlackboardBase* GetBlackboard() const
	{
		return Blackboard;
	}
	
	// Return State Blackboard as a subtype of IRpStateContext.
	// For use in Native code only. 
	template <class ContextSubClass>
	FORCEINLINE ContextSubClass* GetBlackboard() const
	{
		static_assert(std::is_base_of_v<URpStateMachineBlackboardBase, ContextSubClass>, "ContextSubClass is not derived from URPStateMachineBlackboardBase.");
		return Cast<ContextSubClass>(Blackboard);
	}
	
	virtual void OnActivate()
	{
		StartState();
		//BP_OnActivate();
	};
	
	virtual void OnDeactivate()
	{
		//BP_OnDeactivate();
	};
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnActivate();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDeactivate();
	
protected:
	
	bool bHasFinished = false;

	// The state context object of this state that contains all the information that it needs.
	UPROPERTY()
	TObjectPtr<URpStateMachineBlackboardBase> Blackboard;
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	FString StateName;
};
