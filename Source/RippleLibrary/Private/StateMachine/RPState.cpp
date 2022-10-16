// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#include "StateMachine/RPState.h"

#include "Ripple/Public/LoggingMacros.h"
#include "StateMachine/RPStateContext.h"

void URPState::SetContext_Implementation(URPStateContext* Context)
{
	if (StateContext != Context)
	{
		StateContext = Context;
	}
}

void URPState::Execute_Implementation()
{
	Initialize();
	Execute_Internal();
}

void URPState::Initialize_Implementation()
{
	CLOGV(Warning, "Initialize() has not been implemented.");
}

void URPState::Execute_Internal_Implementation()
{
	CLOGV(Warning, "Execute_Internal() has not been implemented.");
}

URPStateContext* URPState::GetContext(TSubclassOf<URPStateContext> ContextSubClass) const
{
	return StateContext;
}
