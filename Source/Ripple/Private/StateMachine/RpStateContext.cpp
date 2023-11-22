// Copyright Anupam Sahu. All Rights Reserved.

#include "StateMachine/RpStateContext.h"
#include "..\..\Public\RpLoggingMacros.h"
#include "StateMachine/RpState.h"

// Checks TargetState
// Sets Current State
// Executes TargetState
void IRpStateContext::GoToState_Implementation(TSubclassOf<URpState> TargetState)
{
	if(!TargetState)
	{
		CLOGV(Warning, "TargetState class is none.");
		return;
	}
	
	const TObjectPtr<URpState>& TargetStateObject = TargetState.GetDefaultObject();
	SetCurrentState(TargetStateObject);

	// Wrapping and sending this interface in a TScriptInterface,
	// because the SetContext() call doesn't accept anything else.
	TScriptInterface<IRpStateContext> TempInterface;
	TempInterface.SetObject(_getUObject());
	TempInterface.SetInterface(this);

	TargetStateObject->SetContext(TempInterface);
	TargetStateObject->Activate();
}

void IRpStateContext::StartStateMachine_Implementation(TSubclassOf<URpState> StartingState)
{
	if(!StartingState)
	{
		CLOGV(Warning, "StartingState is not valid, failed to start state machine.");
		return;
	}
	GoToState_Implementation(StartingState);
}

TObjectPtr<URpState> URpStateContextBase::GetCurrentState() const
{
	return CurrentState;
}

void URpStateContextBase::SetCurrentState(const TObjectPtr<URpState>& Value)
{
	CurrentState = Value;
}
