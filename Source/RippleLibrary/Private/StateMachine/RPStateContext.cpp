// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#include "StateMachine/RPStateContext.h"
#include "Ripple/Public/LoggingMacros.h"
#include "StateMachine/RPState.h"

// Checks TargetState
// Sets Current State
// Executes TargetState
void IRPStateContext::GoToState_Implementation(TSubclassOf<URPState> TargetState)
{
	if(!TargetState)
	{
		CLOGV(Warning, "TargetState class is none.");
		return;
	}
	
	const TObjectPtr<URPState>& TargetStateObject = TargetState.GetDefaultObject();
	SetCurrentState(TargetStateObject);

	// Wrapping and sending this interface in a TScriptInterface,
	// because the SetContext() call doesn't accept anything else.
	TScriptInterface<IRPStateContext> TempInterface;
	TempInterface.SetObject(_getUObject());
	TempInterface.SetInterface(this);

	TargetStateObject->SetContext(TempInterface);
	TargetStateObject->Execute();
}

void IRPStateContext::StartStateMachine_Implementation(TSubclassOf<URPState> StartingState)
{
	if(!StartingState)
	{
		CLOGV(Warning, "StartingState is not valid, failed to start state machine.");
		return;
	}
	GoToState_Implementation(StartingState);
}

TObjectPtr<URPState> URPStateContextBase::GetCurrentState() const
{
	return CurrentState;
}

void URPStateContextBase::SetCurrentState(const TObjectPtr<URPState>& Value)
{
	CurrentState = Value;
}
