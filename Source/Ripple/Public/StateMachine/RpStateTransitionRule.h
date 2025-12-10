// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RpStateTransitionRule.generated.h"

class URpState;
class URpStateMachineBlackboardBase;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class RIPPLE_API URpStateTransitionRule : public UObject
{
	GENERATED_BODY() 

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanTransition() const;
	virtual bool CanTransition_Implementation() const { return false; }
		
	// Set the state context object for this state.
	UFUNCTION(BlueprintCallable)
	void SetBlackboard(URpStateMachineBlackboardBase* NewBlackboard);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<URpState> GetNextState() const
	{
		return EntryState;
	}

	UFUNCTION(BlueprintCallable)
	TSubclassOf<URpState> GetRequiredState() const
	{
		return ExitState;
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
	template <class BlackboardSubClass>
	FORCEINLINE BlackboardSubClass* GetContext() const
	{
		static_assert(std::is_base_of_v<URpStateMachineBlackboardBase, BlackboardSubClass>, "ContextSubClass is not derived from URPStateMachineBlackboardBase.");
		return Cast<BlackboardSubClass>(Blackboard);
	}
	
protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URpState> ExitState;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URpState> EntryState;

	// The state context object of this state that contains all the information that it needs.
	UPROPERTY(Transient)
	TObjectPtr<URpStateMachineBlackboardBase> Blackboard;
};
 
inline void URpStateTransitionRule::SetBlackboard(URpStateMachineBlackboardBase* NewBlackboard)
{
	Blackboard = NewBlackboard;
}
