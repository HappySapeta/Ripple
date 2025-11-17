// Copyright Anupam Sahu. All Rights Reserved.


#include "GOAP/RpGOAPGoal.h"

// TODO : O(mn). Optimization might be required.
bool URpGOAPGoal::IsGoal(const FRpGOAPState& State, const float Tolerance) const
{
	for (const auto& [GoalType, GoalVariant] : GoalState.Facts)
	{
		for (const auto& [StateType, StateVariant] : State.Facts)
		{
			if (!FRpVariant::IsNearlyEqual(StateVariant, GoalVariant, Tolerance))
			{
				return false;
			}
		}
	}
	
	return true;
}
