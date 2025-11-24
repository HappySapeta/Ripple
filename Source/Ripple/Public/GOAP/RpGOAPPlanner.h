// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RpGOAPPlanner.generated.h"

class URpGOAPState;
class URpGOAPGoal;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class RIPPLE_API URpGOAPPlanner : public UObject
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditAnywhere)
	TArray<URpGOAPGoal*> Goals;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URpGOAPState> CurrentState;
};
