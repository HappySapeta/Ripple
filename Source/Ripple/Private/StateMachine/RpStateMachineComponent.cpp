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

void URpStateMachineComponent::Initialize()
{
	Blackboard = NewObject<URpStateMachineBlackboardBase>(this, StatemachineBBClass);
	
	for (const auto& StateClass : StateClasses)
	{
		URpState* NewState = NewObject<URpState>(this, StateClass);
		StateInstances.Add(NewState);
		
		NewState->SetBlackboard(Blackboard);
	}
	
	for (const auto& RuleClass : TransitionRules)
	{
		URpStateTransitionRule* NewRule = NewObject<URpStateTransitionRule>(this, RuleClass);
		TransitionRuleInstances.Add(NewRule);
		
		NewRule->SetBlackboard(Blackboard);
	}
}

URpState* URpStateMachineComponent::FindStateToTransition(const URpStateTransitionRule* Rule)
{
	for (URpState* State : StateInstances)
	{
		if (State->GetClass() == Rule->GetNextState())
		{
			return State;
		}
	}
	
	return nullptr;
}

void URpStateMachineComponent::ProcessRules()
{
	URpState* NextState = nullptr;
	for (const URpStateTransitionRule* Rule : TransitionRuleInstances)
	{
		// Check if entry and exit states match.
		if (Rule->GetRequiredState() != CurrentState->GetClass())
		{
			continue;
		}
		
		// Check if rule is allowed to transition.
		if (!Rule->CanTransition())
		{
			continue;
		}
		
		// Check if rule is allowed to interrupt current state. 
		if (!CurrentState->HasFinshed() && !Rule->AllowInterruption())
		{
			continue;
		}
		
		NextState = FindStateToTransition(Rule);
	}

	if (NextState)
	{
		UE_LOG(LogTemp, Warning,
			   TEXT("[URpStateMachineComponent::ProcessRules] State Change : %s -> %s"),
			   *CurrentState->GetStateName(), *NextState->GetStateName());
		
		CurrentState->DeActivate();
		CurrentState = NextState;
		CurrentState->Activate();
		OnStateChanged.Broadcast(CurrentState->GetStateName());
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