// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#include "StateMachine/RPStateContext.h"
#include "Ripple/Public/LoggingMacros.h"
#include "StateMachine/RPState.h"

void URPStateContext::GoToState(TSubclassOf<URPState> TargetState)
{
	if (TargetState)
	{
		CurrentState = TargetState.GetDefaultObject();
		CurrentState->SetContext(this);
		CurrentState->Execute();
	}
	else
	{
		CLOGV(Warning, "TargetState class is none.");
	}
}

void URPStateContext::StartStateMachine(TSubclassOf<URPState> StartingState)
{
	if(!StartingState)
	{
		CLOGV(Warning, "StartingState is not valid, failed to start state machine.");
		return;
	}

	GoToState(StartingState);
}
