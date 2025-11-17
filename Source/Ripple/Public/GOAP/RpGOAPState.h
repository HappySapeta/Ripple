// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "RpGOAPState.generated.h"

USTRUCT(BlueprintType)
struct FRpVariantBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, DisplayName = "Rp Float")
struct FRpVarFloat : public FRpVariantBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float Value = 0.0f;
};

USTRUCT(BlueprintType, DisplayName = "Rp Integer")
struct FRpVarInteger : public FRpVariantBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	int Value = 0;
};

USTRUCT(BlueprintType, DisplayName = "Rp Bool")
struct FRpVarBool : public FRpVariantBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	bool Value = false;
};

USTRUCT(BlueprintType)
struct FRpVariantWrapper 
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (BaseStruct = "/script/Ripple.RpVariantBase"))
	FInstancedStruct Var;
};
 
USTRUCT(BlueprintType)
struct FRpGOAPState
{
	GENERATED_BODY()

public:
	
	template <typename Type>
	const Type& Get(const FGameplayTag& FactName) const requires TIsDerivedFrom<Type, FRpVariantBase>::IsDerived 
	{
		if (ensureMsgf(Facts.Contains(FactName), TEXT("Failed to find Fact with given name.")))
		{
			const auto Var = Facts[FactName].Var;
			if (ensureMsgf(Var.IsValid(), TEXT("Invalid Struct.")))
			{
				return Facts[FactName].Var.Get<Type>();
			}
		}
		return FRpVariantBase{};
	}
	
	template <typename Type>
	void Set(const FGameplayTag& FactName, const Type& Value) requires TIsDerivedFrom<Type, FRpVariantBase>::IsDerived
	{
		if (ensureMsgf(Facts.Contains(FactName), TEXT("Failed to find Fact with given name.")))
		{
			const auto Var = Facts[FactName].Var;
			if (ensureMsgf(Var.IsValid(), TEXT("Invalid Struct.")))
			{
				Facts[FactName].Var.GetMutable<Type>() = Value;
			}
		}
	}
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FRpVariantWrapper> Facts;
};
