// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RpGOAPState.h"
#include "UObject/Object.h"
#include "RpGOAPAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class RIPPLE_API URpGOAPAction : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual void Perform(const URpGOAPState* TestState);
	
	bool CheckRequirements(const URpGOAPState* State);
	
	float GetCost() const
	{
		return Cost;	
	}
	
	void SetCost(const float NewCost)
	{
		Cost = NewCost;
	}
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnPerformAction")
	void BP_OnPerformAction();

protected:
	
	UPROPERTY(EditAnywhere)
	float Cost = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FRpRequirementDescriptor> Requirements;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FRpStateDescriptor> Effects;
};
