// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GOAP/RpGOAPTypes.h"
#include "RpGOAPGoal.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Category = "Ripple GOAP")
class RIPPLE_API URpGOAPGoal : public UObject
{
	GENERATED_BODY()
	
public:
	
	const TMap<FGameplayTag, FRpRequirementDescriptor>& GetRequirements() const
	{
		return Requirements;
	}
	
	uint8 GetPriority() const
	{
		return Priority;
	}
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FRpRequirementDescriptor> Requirements;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 Priority = 0;
};
