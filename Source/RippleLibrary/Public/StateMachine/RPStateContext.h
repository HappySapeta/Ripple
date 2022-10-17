// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPStateContext.generated.h"

// Forward Declarations
class URPState;

UINTERFACE(Blueprintable)
class URPStateContext : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Ripple Simple State Machine.
 *  RPStateContext defines a State Machine's Context,
 *  that is coupled with RPState and together they
 *  can be used to create a very simple state machine.
 */
class IRPStateContext
{
	GENERATED_BODY()

public:
	
	// Transitions the state machine to TargetState.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GoToState(TSubclassOf<URPState> TargetState);
	void GoToState_Implementation(TSubclassOf<URPState> TargetState);
	
	// Starts the state machine by activating StartingState.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartStateMachine(TSubclassOf<URPState> StartingState);
	void StartStateMachine_Implementation(TSubclassOf<URPState> StartingState);

protected:

	// Get CurrentState.
	virtual TObjectPtr<URPState> GetCurrentState() const = 0;

	// Set CurrentState.
	virtual void SetCurrentState(const TObjectPtr<URPState>& Value) = 0;
};

/**
 * RPStateContextBase derives from RPStateContext interface
 * and it is intended to be used as a base class for
 * classes implementing the RPStateContext interface,
 * especially in blueprints.
 * It also provides a field to store the current state of the State Machine.
 */
UCLASS(Blueprintable, BlueprintType)
class URPStateContextBase : public UObject, public IRPStateContext
{
	GENERATED_BODY()
	
protected:
	
	// Get CurrentState.
	virtual TObjectPtr<URPState> GetCurrentState() const override;

	// Set CurrentState.
	virtual void SetCurrentState(const TObjectPtr<URPState>& Value) override;

protected:

	// Current State of the State Machine.
	UPROPERTY(Transient)
	TObjectPtr<URPState> CurrentState;
};
