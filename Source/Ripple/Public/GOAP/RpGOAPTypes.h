// Copyright Anupam Sahu. All Rights Reserved.

#pragma once
#include "StructUtils/InstancedStruct.h"
#include "RpGOAPTypes.generated.h"

USTRUCT(BlueprintType)
struct FRpVariantBase
{
	GENERATED_BODY()
	
	virtual ~FRpVariantBase() = default;
	
	virtual void Set(const FRpVariantBase*) PURE_VIRTUAL (FRpVariantBase::Set, )
	
	virtual int GetAbsDifference(const FRpVariantBase*) const PURE_VIRTUAL (FRpVariantBase::GetAbsDifference, return 0; );
	virtual bool operator==(const FRpVariantBase*) const PURE_VIRTUAL (FRpVariantBase::operator==, return false; );
	virtual bool operator<(const FRpVariantBase*) const PURE_VIRTUAL (FRpVariantBase::operator<, return false; );
	virtual bool operator>(const FRpVariantBase*) const PURE_VIRTUAL (FRpVariantBase::operator>, return false; );
	
	bool operator!=(const FRpVariantBase* Other) const
	{
		return !(operator==(Other));
	}
};

USTRUCT(BlueprintType, DisplayName = "RpFloatVariant")
struct FRpVariantFloat : public FRpVariantBase
{
	GENERATED_BODY()
	
	virtual ~FRpVariantFloat() = default;
	
	virtual int GetAbsDifference(const FRpVariantBase* Other) const override;
	virtual bool operator==(const FRpVariantBase* Other) const override;
	virtual bool operator<(const FRpVariantBase* Other) const override;
	virtual bool operator>(const FRpVariantBase* Other) const override;
	virtual void Set(const FRpVariantBase*) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0.0f;
};

USTRUCT(BlueprintType, DisplayName = "RpIntegerVariant")
struct FRpVariantInteger : public FRpVariantBase
{
	GENERATED_BODY()
	
	virtual ~FRpVariantInteger() = default;
	
	virtual int GetAbsDifference(const FRpVariantBase*) const override;
	virtual bool operator==(const FRpVariantBase*) const override;
	virtual bool operator<(const FRpVariantBase*) const override;
	virtual bool operator>(const FRpVariantBase*) const override;
	virtual void Set(const FRpVariantBase*) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Value = 0;
};

USTRUCT(BlueprintType, DisplayName = "RpBooleanVariant")
struct FRpVariantBool : public FRpVariantBase
{
	GENERATED_BODY()
	
	virtual ~FRpVariantBool() = default;
	
	virtual int GetAbsDifference(const FRpVariantBase*) const override;
	virtual bool operator==(const FRpVariantBase*) const override;
	virtual bool operator<(const FRpVariantBase*) const override;
	virtual bool operator>(const FRpVariantBase*) const override;
	virtual void Set(const FRpVariantBase*) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Value = false;
};

USTRUCT(BlueprintType, DisplayName = "RpVector3Variant")
struct FRpVariantVector3 : public FRpVariantBase
{
	GENERATED_BODY()
	
	virtual ~FRpVariantVector3() = default;
	
	virtual int GetAbsDifference(const FRpVariantBase*) const override;
	virtual bool operator==(const FRpVariantBase*) const override;
	virtual bool operator<(const FRpVariantBase*) const override;
	virtual bool operator>(const FRpVariantBase*) const override;
	virtual void Set(const FRpVariantBase*) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector3f Value = {0, 0, 0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxComparisonError = 10.0f;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BaseStruct = "/script/Ripple.RpVariantBase"))
	FInstancedStruct Fact;
};

USTRUCT(BlueprintType)
struct FRpRequirementDescriptor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BaseStruct = "/script/Ripple.RpVariantBase"))
	FInstancedStruct Fact;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ERpCondition> Condition = ERpCondition::EQUAL;
};
 