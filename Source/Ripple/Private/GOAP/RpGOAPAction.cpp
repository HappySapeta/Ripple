// Copyright Anupam Sahu. All Rights Reserved.
#include "GOAP/RpGOAPAction.h"

void URpGOAPAction::Perform(const URpGOAPState* TestState)
{
	if (!CheckRequirements(TestState))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to perform GOAP Action. Requirements are not met."));
		return;
	}
	
	BP_OnPerformAction();
}

bool URpGOAPAction::CheckRequirements(const URpGOAPState* TestState)
{
	for (const auto& [FactName, Descriptor] : Requirements)
	{
		if (!TestState->Contains(FactName))
		{
			return false;
		}

		switch (Descriptor.Condition)
		{
			case EQUAL:
			{
				const bool bResult = Descriptor.Fact.GetPtr<FRpVariantBase>()->operator==(TestState->Get(FactName));
				if (bResult == false)
				{
					return false;
				}
			}
			case LESS:
			{
				const bool bResult = Descriptor.Fact.GetPtr<FRpVariantBase>()->operator<(TestState->Get(FactName));
				if (bResult == false)
				{
					return false;
				}
			}
			case GREATER:
			{
				const bool bResult = Descriptor.Fact.GetPtr<FRpVariantBase>()->operator>(TestState->Get(FactName));
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
