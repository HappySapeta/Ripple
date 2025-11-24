// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RpGOAPState.h"
#include "RpGOAPGoal.generated.h"

/**
 * 
 */
UCLASS()
class RIPPLE_API URpGOAPGoal : public UDataAsset
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FRpRequirementDescriptor> Requirements;
};
