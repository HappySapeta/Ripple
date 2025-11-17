// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RpGOAPState.h"
#include "UObject/Object.h"
#include "RpGOAPGoal.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class RIPPLE_API URpGOAPGoal : public UObject
{
	GENERATED_BODY()

public:

	FRpGOAPState& ModifyState()
	{
		return GoalState;
	}

	const FRpGOAPState& GetState() const
	{
		return GoalState;
	}
	
	bool IsGoal(const FRpGOAPState& State, const float Tolerance = 0.00001f) const;
	
protected:

	UPROPERTY(EditDefaultsOnly)
	FRpGOAPState GoalState;
};
