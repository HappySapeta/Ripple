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
 * Derived classes provide their own implementation for
 * 2 functions that represent 2 primary stages of a state :
 * Enter() and Execute()
 */
UCLASS(Blueprintable)
class URPState : public UObject
{
	GENERATED_BODY()

public:
	// Set the state context object for this state.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetContext(URPStateContext* Context);

	// Invokes this state. Used for initialization purposes
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Enter();

protected:
	// Contains the primary logic of this state.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Execute();

	// Return StateContext as a subtype of URPStateContext. (Blueprint use only)
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ContextSubClass"))
	URPStateContext* GetContext(TSubclassOf<URPStateContext> ContextSubClass) const
	{
		return StateContext;
	}

	// Return StateContext as a subtype of URPStateContext.
	template<class ContextSubClass>
	FORCEINLINE ContextSubClass* GetContext() const
	{
		static_assert(std::is_base_of_v<URPStateContext, ContextSubClass>, "ContextSubClass is not derived from URPStateContext.");
		return Cast<ContextSubClass>(StateContext);
	}
	
private:
	// The state context object of this state that contains all the information that it needs.
	UPROPERTY(Transient)
	TObjectPtr<URPStateContext> StateContext;
};
