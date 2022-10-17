// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#include "StateMachine/RPState.h"
#include "Ripple/Public/LoggingMacros.h"
#include "StateMachine/RPStateContext.h"

void URPState::SetContext_Implementation(const TScriptInterface<IRPStateContext>& Context)
{
	// Checks if the Context object is different, and sets the current state context.
	if(StateContext.GetObject() != Context.GetObject())
	{
		StateContext.SetObject(Context.GetObject());
		StateContext.SetInterface(Context.GetInterface());
	}
}

void URPState::Execute_Implementation()
{
	Initialize();
	Execute_Internal();
}

// Add initialization logic : set references, reset flags etc. 
void URPState::Initialize_Implementation()
{
	CLOGV(Warning, "Initialize() has not been implemented.");
}

// Add core execution logic here
void URPState::Execute_Internal_Implementation()
{
	CLOGV(Warning, "Execute_Internal() has not been implemented.");
}

// Casts the StateContext reference into the requested type and provides a pin on its blueprint node to let the user choose a return type.
URPStateContextBase* URPState::GetContext(TSubclassOf<URPStateContextBase> ContextSubClass) const
{
	return Cast<URPStateContextBase>(StateContext.GetInterface());
}
