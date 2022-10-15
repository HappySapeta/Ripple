// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#include "StateMachine/RPState.h"

#include "Ripple/Public/LoggingMacros.h"
#include "StateMachine/RPStateContext.h"

void URPState::SetContext_Implementation(URPStateContext* Context)
{
	if(StateContext != Context)
	{
		StateContext = Context;
	}
}

void URPState::Enter_Implementation()
{
	CLOGV(Warning, "Enter() has not been implemented.");
}

void URPState::Execute_Implementation()
{
	CLOGV(Warning, "Execute() has not been implemented.");
}
