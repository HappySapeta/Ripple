// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "RpGOAPTypes.h"
#include "RpGOAPAction.generated.h"

class URpGOAPState;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class RIPPLE_API URpGOAPAction : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual void Perform(URpGOAPState* State);
	
	float GetCost() const
	{
		return Cost;	
	}
	
	void SetCost(const float NewCost)
	{
		Cost = NewCost;
	}
	
	const TMap<FGameplayTag, FRpRequirementDescriptor>& GetRequirements() const
	{
		return Requirements;
	}
	
	const FFactsContainer& GetEffects() const
	{
		return Effects;
	}
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnPerformAction")
	void BP_OnPerformAction();
	
	UFUNCTION(BlueprintCallable)
	void OnActionComplete();

protected:
	
	UPROPERTY(EditAnywhere)
	float Cost = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FRpRequirementDescriptor> Requirements;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FRpStateDescriptor> Effects;
	
	UPROPERTY(BlueprintReadOnly)
	URpGOAPState* TargetState;
};
