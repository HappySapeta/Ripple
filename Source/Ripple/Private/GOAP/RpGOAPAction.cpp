// Copyright Anupam Sahu. All Rights Reserved.
#include "GOAP/RpGOAPAction.h"
#include "GOAP/RpGOAPState.h"

void URpGOAPAction::Perform(URpGOAPState* State)
{
	BP_OnPerformAction();
	TargetState = State;
}

void URpGOAPAction::OnActionComplete()
{
	for (const auto& [FactName, EffectDescriptor] : GetEffects())
	{
		TargetState->SetFact(FactName, EffectDescriptor);
	}
}
