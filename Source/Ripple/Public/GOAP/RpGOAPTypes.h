// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "StructUtils/InstancedStruct.h"
#include "RpGOAPTypes.generated.h"

typedef TMap<FGameplayTag, FRpStateDescriptor> FFactsContainer;

USTRUCT(BlueprintType)
struct FRpVariantBase
{
	GENERATED_BODY()
	
	virtual ~FRpVariantBase() = default;
	
	virtual void Set(const FRpVariantBase*) PURE_VIRTUAL (FRpVariantBase::Set, );
	
	virtual int GetAbsDifference(const FRpVariantBase*) const PURE_VIRTUAL (FRpVariantBase::GetAbsDifference, return 0; );
	virtual bool operator==(const FRpVariantBase*) const PURE_VIRTUAL (FRpVariantBase::operator==, return false; );
	virtual bool operator<(const FRpVariantBase*) const PURE_VIRTUAL (FRpVariantBase::operator<, return false; );
	virtual bool operator>(const FRpVariantBase*) const PURE_VIRTUAL (FRpVariantBase::operator>, return false; );
};

USTRUCT(BlueprintType, DisplayName = "Floating Point")
struct FRpVariantFloat : public FRpVariantBase
{
	GENERATED_BODY()
	
	virtual ~FRpVariantFloat() = default;
	
	virtual int GetAbsDifference(const FRpVariantBase* Other) const override;
	virtual bool operator==(const FRpVariantBase* Other) const override;
	virtual bool operator<(const FRpVariantBase* Other) const override;
	virtual bool operator>(const FRpVariantBase* Other) const override;
	virtual void Set(const FRpVariantBase*) override;

	UPROPERTY(EditAnywhere)
	float Value = 0.0f;
};

USTRUCT(BlueprintType, DisplayName = "Integer")
struct FRpVariantInteger : public FRpVariantBase
{
	GENERATED_BODY()
	
	virtual ~FRpVariantInteger() = default;
	
	virtual int GetAbsDifference(const FRpVariantBase*) const override;
	virtual bool operator==(const FRpVariantBase*) const override;
	virtual bool operator<(const FRpVariantBase*) const override;
	virtual bool operator>(const FRpVariantBase*) const override;
	virtual void Set(const FRpVariantBase*) override;
	
	UPROPERTY(EditAnywhere)
	int Value = 0;
};

USTRUCT(BlueprintType, DisplayName = "Boolean")
struct FRpVariantBool : public FRpVariantBase
{
	GENERATED_BODY()
	
	virtual ~FRpVariantBool() = default;
	
	virtual int GetAbsDifference(const FRpVariantBase*) const override;
	virtual bool operator==(const FRpVariantBase*) const override;
	virtual bool operator<(const FRpVariantBase*) const override;
	virtual bool operator>(const FRpVariantBase*) const override;
	virtual void Set(const FRpVariantBase*) override;
	
	UPROPERTY(EditAnywhere)
	bool Value = false;
};

UENUM()
enum ERpCondition : uint8
{
	// REQUIRED = TARGET
	EQUAL,
	
	// REQUIRED < TARGET
	LESS,
	
	// REQUIRED > TARGET
	GREATER
};

USTRUCT(BlueprintType)
struct FRpStateDescriptor 
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (BaseStruct = "/script/Ripple.RpVariantBase"))
	FInstancedStruct Fact;
};

USTRUCT(BlueprintType)
struct FRpRequirementDescriptor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (BaseStruct = "/script/Ripple.RpVariantBase"))
	FInstancedStruct Fact;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERpCondition> Condition = ERpCondition::EQUAL;
};
 