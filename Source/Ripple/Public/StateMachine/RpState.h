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
	void SetContext(URpStateMachineBlackboardBase* NewBlackboard)
	{
		Blackboard = NewBlackboard;
	}
	
	void Activate();
	void DeActivate();
	FString GetStateName() const
	{
		return StateName;
	}

protected:

	// Return State Blackboard as a subtype of IRpStateContext.
	// For use in blueprints only.
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ContextSubClass"))
	URpStateMachineBlackboardBase* GetContext(TSubclassOf<URpStateMachineBlackboardBase> ContextSubClass) const
	{
		return Blackboard;
	}
	
	// Return State Blackboard as a subtype of IRpStateContext.
	// For use in Native code only. 
	template <class ContextSubClass>
	FORCEINLINE ContextSubClass* GetContext() const
	{
		static_assert(std::is_base_of_v<URpStateMachineBlackboardBase, ContextSubClass>, "ContextSubClass is not derived from URPStateMachineBlackboardBase.");
		return Cast<ContextSubClass>(Blackboard);
	}
	
	UFUNCTION(BlueprintNativeEvent)
	void OnActivate();
	virtual void OnActivate_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void OnDeactivate();
	virtual void OnDeactivate_Implementation() {}

private:
	
	// The state context object of this state that contains all the information that it needs.
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URpStateMachineBlackboardBase> Blackboard;
	
	UPROPERTY(EditDefaultsOnly)
	FString StateName;
};
