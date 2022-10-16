// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPState.generated.h"

// Forward declarations
class URPStateContext;

/**
 * Ripple Simple State Machine.
 * UState represents a state of a state machine.
 * All data is received from a StateContext object.
 * Derived states provide implementation logic
 * for state Initialization and Execution.
 */
UCLASS(Blueprintable)
class URPState : public UObject
{
	GENERATED_BODY()

public:
	// Set the state context object for this state.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetContext(URPStateContext* Context);

	// Puts the state into action.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Execute();

protected:
	// Initialize references, and reset state variables. 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Initialize();

	// Execute core logic.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Execute_Internal();

	// Return StateContext as a subtype of URPStateContext. (Blueprint use only)
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ContextSubClass"))
	URPStateContext* GetContext(TSubclassOf<URPStateContext> ContextSubClass) const;

	// Return StateContext as a subtype of URPStateContext.
	template <class ContextSubClass>
	FORCEINLINE ContextSubClass* GetContext() const
	{
		static_assert(std::is_base_of_v<URPStateContext, ContextSubClass>,
			"ContextSubClass is not derived from URPStateContext.");
		return Cast<ContextSubClass>(StateContext);
	}

private:
	// The state context object of this state that contains all the information that it needs.
	UPROPERTY(Transient)
	TObjectPtr<URPStateContext> StateContext;
};
