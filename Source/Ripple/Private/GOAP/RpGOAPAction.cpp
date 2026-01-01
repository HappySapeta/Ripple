// Copyright Anupam Sahu. All Rights Reserved.
#include "GOAP/RpGOAPAction.h"
#include "GOAP/RpGOAPState.h"

void URpGOAPAction::Execute(URpGOAPState* State)
{
	TargetState = State;
	BP_OnPerformAction();
}

void URpGOAPAction::OnActionComplete()
{
	for (const auto& [FactName, EffectDescriptor] : GetEffects())
	{
		TargetState->SetFact(FactName, EffectDescriptor);
	}
	
	if (NextActionToRun)
	{
		NextActionToRun->Execute(TargetState);
	}
	
	OnActionCompleteEvent.Broadcast(TargetState);	
}
