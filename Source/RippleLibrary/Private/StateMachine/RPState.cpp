// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#include "StateMachine/RPState.h"
#include "StateMachine/RPStateContext.h"

void URPState::SetContext_Implementation(const TScriptInterface<IRPStateContext>& Context)
{
	if(StateContext.GetObject() != Context.GetObject())
	{
		StateContext.SetObject(Context.GetObject());
		StateContext.SetInterface(Context.GetInterface());
	}
}

void URPState::Activate_Implementation()
{
	// Do Nothing.
}

// Casts the StateContext reference into the requested type and provides a pin on its blueprint node to let the user choose a return type.
URPStateContextBase* URPState::GetContext(TSubclassOf<URPStateContextBase> ContextSubClass) const
{
	return Cast<URPStateContextBase>(StateContext.GetInterface());
}
