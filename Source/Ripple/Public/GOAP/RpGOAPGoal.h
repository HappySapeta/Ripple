// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RpGOAPState.h"
#include "RpGOAPGoal.generated.h"

/**
 * 
 */
UCLASS(Category = "Ripple GOAP")
class RIPPLE_API URpGOAPGoal : public UDataAsset
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FRpRequirementDescriptor> Requirements;
};
