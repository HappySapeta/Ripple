// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPStateContext.generated.h"

// Forward Declarations
class URPState;

/**
 *  Ripple Simple State Machine.
 *  RPStateContext defines a State Machine's Context,
 *  that is coupled with RPState and together they
 *  can be used to create a very simple state machine.
 */
UCLASS(Blueprintable)
class URPStateContext : public UObject
{
	GENERATED_BODY()

public:
	// Transitions the state machine to TargetState.
	UFUNCTION(BlueprintCallable)
	virtual void GoToState(TSubclassOf<URPState> TargetState);

	// Starts the state machine by activating StartingState.
	UFUNCTION(BlueprintCallable)
	void StartStateMachine(TSubclassOf<URPState> StartingState);

protected:
	// A reference to the current state of the state machine.
	UPROPERTY(Transient, BlueprintReadWrite)
	TObjectPtr<URPState> CurrentState;
};
