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

	void SetRequirements(const TMap<FGameplayTag, FRpRequirementDescriptor>& NewRequirements)
	{
		Requirements = NewRequirements;
	}
	
	UFUNCTION(BlueprintCallable)
	const UScriptStruct* GetScriptStruct(const FGameplayTag& FactName) const
	{
		if (Requirements.Contains(FactName))
		{
			return Requirements[FactName].Fact.GetScriptStruct();
		}
	
		return nullptr;
	}
	
	UFUNCTION(BlueprintCallable)
	void SetRequirement(const FGameplayTag& FactName, const FRpRequirementDescriptor& Value)
	{
		if (Requirements.Contains(FactName))
		{
			if (ensureAlwaysMsgf(GetScriptStruct(FactName) == Value.Fact.GetScriptStruct(),TEXT("ScriptStructs do not match")))
			{
				Requirements[FactName] = Value;
			}
		}
	}

	void PrintRequirements(const FString& Label)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Label);
		for (const auto& [FactName, Descriptor] : Requirements)
		{
			UE_LOG(LogTemp, Log, TEXT("Rqmt %s : %s"), *FactName.ToString() , *Descriptor.ToString());
		}
	}

	bool operator==(const URpGOAPGoal& Other) const
	{
		if (Requirements.Num() != Other.Requirements.Num())
		{
			return false;
		}
		
		for (const auto& [FactName, Descriptor] : Requirements)
		{
			if (!Other.Requirements.Contains(FactName))
			{
				return false;
			}

			const FRpRequirementDescriptor& OtherDescriptor = Other.Requirements[FactName];
			if (Descriptor.ToString() != OtherDescriptor.ToString() || Descriptor.Condition != OtherDescriptor.Condition)
			{
				return false;
			}
		}
		
		return true;
	}

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FRpRequirementDescriptor> Requirements;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 Priority = 0;
};
