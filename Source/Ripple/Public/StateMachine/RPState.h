// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RpState.generated.h"

// Forward declarations
class IRpStateContext;
class URpStateContextBase;

/**
 * Ripple Simple State Machine.
 * UState represents a state of a state machine.
 * All data is received from a StateContext object.
 * Derived states provide implementation logic
 * for state Initialization and Execution.
 */
UCLASS(Blueprintable)
class RIPPLE_API URpState : public UObject
{
	
	GENERATED_BODY()

public:
	
	// Set the state context object for this state.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetContext(const TScriptInterface<IRpStateContext>& Context);
	
	// Puts the state into action.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Activate();
	
protected:

	// Return StateContext as a subtype of IRpStateContext.
	// For use in blueprints only.
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ContextSubClass"))
	URpStateContextBase* GetContext(TSubclassOf<URpStateContextBase> ContextSubClass) const;
	
	// Return StateContext as a subtype of IRpStateContext.
	// For use in Native code only. 
	template <class ContextSubClass>
	FORCEINLINE ContextSubClass* GetContext() const
	{
		static_assert(std::is_base_of_v<IRpStateContext, ContextSubClass>, "ContextSubClass is not derived from URpStateContext.");
		return Cast<ContextSubClass>(StateContext.GetInterface());
	}

private:
	// The state context object of this state that contains all the information that it needs.
	UPROPERTY(Transient)
	TScriptInterface<IRpStateContext> StateContext;
};
