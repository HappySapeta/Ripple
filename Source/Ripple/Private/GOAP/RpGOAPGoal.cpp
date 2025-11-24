// Copyright Anupam Sahu. All Rights Reserved.

#include "GOAP/RpGOAPGoal.h"
#include "GOAP/RpGOAPState.h"

bool URpGOAPGoal::Evaluate(const URpGOAPState* TestState)
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
