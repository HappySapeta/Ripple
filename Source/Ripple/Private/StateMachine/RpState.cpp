// Copyright Anupam Sahu. All Rights Reserved.

#include "StateMachine/RpState.h"
#include "StateMachine/RpStateContext.h"

void URpState::SetContext_Implementation(const TScriptInterface<IRpStateContext>& Context)
{
	if(StateContext.GetObject() != Context.GetObject())
	{
		StateContext.SetObject(Context.GetObject());
		StateContext.SetInterface(Context.GetInterface());
	}
}

void URpState::Activate_Implementation()
{
	// Do Nothing.
}

// Casts the StateContext reference into the requested type and provides a pin on its blueprint node to let the user choose a return type.
URpStateContextBase* URpState::GetContext(TSubclassOf<URpStateContextBase> ContextSubClass) const
{
	return Cast<URpStateContextBase>(StateContext.GetInterface());
}
