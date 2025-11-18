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
	
	virtual ~FRpVariantBase() = default;
	
	virtual bool operator==(const FRpVariantBase*) const PURE_VIRTUAL (FRpVariantBase::operator==, return false; );
	virtual bool operator<(const FRpVariantBase*) const PURE_VIRTUAL (FRpVariantBase::operator<, return false; );
	virtual bool operator>(const FRpVariantBase*) const PURE_VIRTUAL (FRpVariantBase::operator>, return false; );
};

USTRUCT(BlueprintType, DisplayName = "Floating Point")
struct FRpVariantFloat : public FRpVariantBase
{
	GENERATED_BODY()
	
	virtual ~FRpVariantFloat() = default;
	
	virtual bool operator==(const FRpVariantBase* Other) const override;
	virtual bool operator<(const FRpVariantBase* Other) const override;
	virtual bool operator>(const FRpVariantBase* Other) const override;

	UPROPERTY(EditAnywhere)
	float Value = 0.0f;
};

USTRUCT(BlueprintType, DisplayName = "Integer")
struct FRpVariantInteger : public FRpVariantBase
{
	GENERATED_BODY()
	
	virtual ~FRpVariantInteger() = default;
	
	virtual bool operator==(const FRpVariantBase*) const override;
	virtual bool operator<(const FRpVariantBase*) const override;
	virtual bool operator>(const FRpVariantBase*) const override;
	
	UPROPERTY(EditAnywhere)
	int Value = 0;
};

USTRUCT(BlueprintType, DisplayName = "Boolean")
struct FRpVariantBool : public FRpVariantBase
{
	GENERATED_BODY()
	
	virtual ~FRpVariantBool() = default;
	
	virtual bool operator==(const FRpVariantBase*) const override;
	virtual bool operator<(const FRpVariantBase*) const override;
	virtual bool operator>(const FRpVariantBase*) const override;
	
	UPROPERTY(EditAnywhere)
	bool Value = false;
};

UENUM()
enum ERpCondition : uint8
{
	EQUAL,  // Equal to requirement
	LESS,   // Less than requirement
	GREATER // Greater than requirement
};

USTRUCT(BlueprintType)
struct FRpStateDescriptor 
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (BaseStruct = "/script/Ripple.RpVariantBase"))
	FInstancedStruct Fact;
};

USTRUCT(BlueprintType)
struct FRpGoalDescriptor
{
	GENERATED_BODY()
	
	bool Evaluate(const FRpStateDescriptor& State) const
	{
		// TODO : Pending implementation
		return false;
	}
	
	UPROPERTY(EditAnywhere, meta = (BaseStruct = "/script/Ripple.RpVariantBase"))
	FInstancedStruct Fact;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERpCondition> Condition = ERpCondition::EQUAL;
	
};
 
UCLASS(Blueprintable)
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

UCLASS(Blueprintable, BlueprintType)
class RIPPLE_API URpGOAPGoalState : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	bool Evaluate(const URpGOAPState* TestState);
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FRpGoalDescriptor> Requirements;
};