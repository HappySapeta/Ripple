// Copyright Anupam Sahu. All Rights Reserved.

#include "StateMachine/RpStateMachineComponent.h"
#include "StateMachine/RpState.h"
#include "StateMachine/RpStateTransitionRule.h"

URpStateMachineComponent::URpStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URpStateMachineComponent::Start()
{
	CurrentState = StartingState.GetDefaultObject();
	CurrentState->Activate();
}

void URpStateMachineComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(CurrentState))
	{
		CurrentState->StateUpdate(DeltaTime);
		ProcessRules();
	}
}

void URpStateMachineComponent::ProcessRules()
{
	URpState* NextState = nullptr;
	for (const TSubclassOf<URpStateTransitionRule>& RuleSubclass : TransitionRules)
	{
		const URpStateTransitionRule* Rule = RuleSubclass.GetDefaultObject();
		if (Rule->GetRequiredState().GetDefaultObject() != CurrentState)
		{
			continue;
		}
			
		if (Rule->CanTransition())
		{
			NextState = Rule->GetNextState().GetDefaultObject();
			break;
		}
	}

	if (NextState)
	{
		CurrentState->DeActivate();
		CurrentState = NextState;
		CurrentState->Activate();
	}
}