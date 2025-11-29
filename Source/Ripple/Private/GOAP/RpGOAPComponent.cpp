// Copyright Anupam Sahu. All Rights Reserved.

#include "GOAP/RpGOAPComponent.h"

#include "GOAP/RpGOAPPlanner.h"

URpGOAPComponent::URpGOAPComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

URpGOAPPlanner* URpGOAPComponent::GetPlanner()
{
	return PlannerClass.GetDefaultObject();
}



