// Copyright Anupam Sahu. All Rights Reserved.

#include "GOAP/RpGOAPPlanner.h"
#include "GOAP/RpGOAPAction.h"

// O(n) : n = number of goals.
const URpGOAPGoal* URpGOAPPlanner::PickGoal()
{
	const URpGOAPGoal* ChosenGoal = nullptr;
	for (const URpGOAPGoal* PotentialGoal : Goals)
	{
		if (!IsValid(ChosenGoal))
		{
			ChosenGoal = PotentialGoal;
			continue;
		}
		
		// Ignore goal if it has currently been achieved.
		if (AreRequirementsSatisfied(PotentialGoal->GetRequirements(), CurrentState))
		{
			continue;
		}
		
		if (ChosenGoal->GetPriority() < PotentialGoal->GetPriority())
		{
			ChosenGoal = PotentialGoal;			
		}
		else if (ChosenGoal->GetPriority() == PotentialGoal->GetPriority())
		{
			if (ChosenGoal->GetRequirements().Num() < PotentialGoal->GetRequirements().Num())
			{
				ChosenGoal = PotentialGoal;
			}
		}
	}
	
	return ChosenGoal;	
}

void URpGOAPPlanner::CreatePlan(const URpGOAPGoal* ChosenGoal)
{
	CurrentGoal = ChosenGoal;
}

bool URpGOAPPlanner::AreRequirementsSatisfied(const TMap<FGameplayTag, FRpRequirementDescriptor>& Requirements,
											 const URpGOAPState* State) const
{
	for (const auto& [FactName, Requirement] : Requirements)
	{
		if (!State->Contains(FactName))
		{
			return false;
		}

		switch (Requirement.Condition)
		{
			case EQUAL:
			{
				const bool bResult = Requirement.Fact.GetPtr<FRpVariantBase>()->operator==(State->Get(FactName));
				if (bResult == false)
				{
					return false;
				}
			}
			case LESS:
			{
				const bool bResult = Requirement.Fact.GetPtr<FRpVariantBase>()->operator>(State->Get(FactName));
				if (bResult == false)
				{
					return false;
				}
			}
			case GREATER:
			{
				const bool bResult = Requirement.Fact.GetPtr<FRpVariantBase>()->operator<(State->Get(FactName));
				if (bResult == false)
				{
					return false;
				}
			}
			default:
				break;
		}
	}
	return true;
}

const URpGOAPState* URpGOAPPlanner::Simulate(const URpGOAPState* Input, const URpGOAPAction* Action)
{
	URpGOAPState* StateCopy = DuplicateObject(Input, GetOuter());
	for (const auto& [FactName, EffectDescriptor] : Action->GetEffects())
	{
		StateCopy->Set(FactName, EffectDescriptor);
	}
	
	return StateCopy;
}
