// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RpGOAPTypes.h"
#include "GameplayTagContainer.h"
#include "RpGOAPState.generated.h"

UCLASS(Blueprintable, Category = "Ripple GOAP")
class URpGOAPState : public UObject
{
	GENERATED_BODY()
	
public:
	
	bool Contains(const FGameplayTag& FactName) const
	{
		return Facts.Contains(FactName);
	}
	
	const FRpVariantBase* Get(const FGameplayTag& FactName) const 
	{
		if (ensureMsgf(Facts.Contains(FactName), TEXT("Failed to find Fact with given name.")))
		{
			const auto Var = Facts[FactName].Fact;
			if (ensureMsgf(Var.IsValid(), TEXT("Invalid Struct.")))
			{
				return Facts[FactName].Fact.GetPtr<FRpVariantBase>();
			}
		}
		
		return nullptr;
	}
	
	template <typename Type>
	const Type& Get(const FGameplayTag& FactName) const requires TIsDerivedFrom<Type, FRpVariantBase>::IsDerived 
	{
		if (ensureMsgf(Facts.Contains(FactName), TEXT("Failed to find Fact with given name.")))
		{
			const auto Var = Facts[FactName].Fact;
			if (ensureMsgf(Var.IsValid(), TEXT("Invalid Struct.")))
			{
				return Facts[FactName].Fact.Get<Type>();
			}
		}
		return FRpVariantBase{};
	}
	
	template <typename Type>
	void Set(const FGameplayTag& FactName, const Type& Value) requires TIsDerivedFrom<Type, FRpVariantBase>::IsDerived
	{
		if (ensureMsgf(Facts.Contains(FactName), TEXT("Failed to find Fact with given name.")))
		{
			const auto Var = Facts[FactName].Fact;
			if (ensureMsgf(Var.IsValid(), TEXT("Invalid Struct.")))
			{
				Facts[FactName].Fact.GetMutable<Type>() = Value;
			}
		}
	}
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FRpStateDescriptor> Facts;
};