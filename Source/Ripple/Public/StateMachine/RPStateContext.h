// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RpStateContext.generated.h"

// Forward Declarations
class URpState;

UINTERFACE(Blueprintable)
class URpStateContext : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Ripple Simple State Machine.
 *  RPStateContext defines a State Machine's Context,
 *  that is coupled with RPState and together they
 *  can be used to create a very simple state machine.
 */
class RIPPLE_API IRpStateContext
{
	GENERATED_BODY()

public:
	
	// Transitions the state machine to TargetState.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GoToState(TSubclassOf<URpState> TargetState);
	void GoToState_Implementation(TSubclassOf<URpState> TargetState);
	
	// Starts the state machine by activating StartingState.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartStateMachine(TSubclassOf<URpState> StartingState);
	void StartStateMachine_Implementation(TSubclassOf<URpState> StartingState);

protected:

	// Get CurrentState.
	virtual TObjectPtr<URpState> GetCurrentState() const = 0;

	// Set CurrentState.
	virtual void SetCurrentState(const TObjectPtr<URpState>& Value) = 0;
};

/**
 * RPStateContextBase derives from RPStateContext interface
 * and it is intended to be used as a base class for
 * classes implementing the RPStateContext interface,
 * especially in blueprints.
 * It also provides a field to store the current state of the State Machine.
 */
UCLASS(Blueprintable, BlueprintType)
class RIPPLE_API URpStateContextBase : public UObject, public IRpStateContext
{
	GENERATED_BODY()
	
protected:
	
	// Get CurrentState.
	virtual TObjectPtr<URpState> GetCurrentState() const override;

	// Set CurrentState.
	virtual void SetCurrentState(const TObjectPtr<URpState>& Value) override;

protected:

	// Current State of the State Machine.
	UPROPERTY(Transient)
	TObjectPtr<URpState> CurrentState;
};
