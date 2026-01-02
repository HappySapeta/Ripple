// Copyright Anupam Sahu. All Rights Reserved.
#include "GOAP/RpGOAPAction.h"
#include "GOAP/RpGOAPState.h"

void URpGOAPAction::Execute(URpGOAPState* State)
{
	TargetState = State;
	BP_OnPerformAction();
}

void URpGOAPAction::Simulate_Implementation(URpGOAPState* StateCopy)
{
	for (const auto& [FactName, EffectDescriptor] : GetEffects())
	{
		StateCopy->SetFact(FactName, EffectDescriptor);
	}
}

void URpGOAPAction::ApplyEffects_Implementation()
{
	for (const auto& [FactName, EffectDescriptor] : GetEffects())
	{
		TargetState->SetFact(FactName, EffectDescriptor);
	}
}

void URpGOAPAction::OnActionComplete()
{
	ApplyEffects();
	
	if (NextActionToRun)
	{
		NextActionToRun->Execute(TargetState);
	}
	
	OnActionCompleteEvent.Broadcast(TargetState);	
}
