// Copyright Anupam Sahu. All Rights Reserved.

#include "StateMachine/RpStateMachineComponent.h"
#include "StateMachine/RpState.h"
#include "StateMachine/RpStateMachineBlackboard.h"
#include "StateMachine/RpStateTransitionRule.h"

URpStateMachineComponent::URpStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URpStateMachineComponent::Start()
{
	for (URpState* State : StateInstances)
	{
		if (State->GetClass() == StartingState)
		{
			CurrentState = State;
			CurrentState->Activate();
		}
	}
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

void URpStateMachineComponent::BeginPlay()
{
	StateMachineBlackboard = NewObject<URpStateMachineBlackboardBase>(GetTransientPackage(), StatemachineBBClass);
	
	for (const auto& StateClass : StateClasses)
	{
		URpState* NewState = NewObject<URpState>(GetTransientPackage(), StateClass);
		StateInstances.Add(NewState);
		
		NewState->SetContext(StateMachineBlackboard);
	}
	
	for (const auto& RuleClass : TransitionRules)
	{
		URpStateTransitionRule* NewRule = NewObject<URpStateTransitionRule>(GetTransientPackage(), RuleClass);
		TransitionRuleInstances.Add(NewRule);
		
		NewRule->SetBlackboard(StateMachineBlackboard);
	}
	
	Super::BeginPlay();
}

void URpStateMachineComponent::ProcessRules()
{
	URpState* NextState = nullptr;
	for (const URpStateTransitionRule* Rule : TransitionRuleInstances)
	{
		if (Rule->GetRequiredState() != CurrentState->GetClass())
		{
			continue;
		}
			
		if (Rule->CanTransition())
		{
			for (URpState* State : StateInstances)
			{
				if (State->GetClass() == Rule->GetNextState())
				{
					NextState = State;
					break;
				}
			}
		}
	}

	if (NextState)
	{
		CurrentState->DeActivate();
		CurrentState = NextState;
		CurrentState->Activate();
		OnStateChanged.Broadcast(CurrentState->GetStateName());
	}
}
