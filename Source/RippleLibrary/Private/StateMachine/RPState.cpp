// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#include "StateMachine/RPState.h"

#include "Ripple/Public/LoggingMacros.h"
#include "StateMachine/RPStateContext.h"

URPState::URPState()
{
	StateContext = CreateDefaultSubobject<URPStateContext>(FName("DefaultStateContext"));
}

void URPState::SetContext_Implementation(URPStateContext* Context)
{
	if(StateContext != Context)
	{
		StateContext = Context;
	}
}

void URPState::Enter_Implementation()
{
	CLOG(Warning, "Using default implementation for Enter().");
}

void URPState::Execute_Implementation()
{
	CLOG(Warning, "Using default implementation for Execute().");
}

void URPState::Exit_Implementation()
{
	CLOG(Warning, "Using default implementation for Exit().");
}
