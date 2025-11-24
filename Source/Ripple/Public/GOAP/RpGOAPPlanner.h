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
UCLASS(Blueprintable, BlueprintType, Category = "Ripple GOAP")
class RIPPLE_API URpGOAPPlanner : public UObject
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TArray<URpGOAPGoal*> Goals;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URpGOAPState> StartingState;
	
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<URpGOAPState> CurrentState;
};
