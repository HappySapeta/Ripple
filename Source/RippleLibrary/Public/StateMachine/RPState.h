﻿// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPState.generated.h"

// Forward declarations
class URPStateContext;

/**
 * Ripple Simple State Machine.
 * UStaticState describes a static state of a state machine.
 * All data is received from the StateContext object.
 * Derived classes provide their own implementation for
 * 3 functions that represent 3 primary stages of a state :
 * Enter(), Execute(), and Exit().
 */
UCLASS(Blueprintable)
class URPState : public UObject
{
	GENERATED_BODY()

public:
	// Initializes the StateContext reference.
	URPState();

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

	// Called after execution.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Exit();

protected:
	// The state context object of this state that contains all the information that it needs.
	UPROPERTY(Transient, BlueprintReadWrite)
	TObjectPtr<URPStateContext> StateContext;
};