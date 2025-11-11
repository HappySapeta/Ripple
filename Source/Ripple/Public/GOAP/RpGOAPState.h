// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Misc/TVariant.h"
#include "RpGOAPState.generated.h"

UENUM()
enum ERpVariantType : uint8
{
	Float,
	Int,
	Bool,
	None
};

USTRUCT()
struct FRpVariant
{
	GENERATED_BODY()

	void Set(ERpVariantType RequestedType, const void* Value)
	{
		switch (RequestedType)
		{
			case ERpVariantType::Float:
			{
				FloatValue = *static_cast<const float*>(Value);
			}
			case ERpVariantType::Int:
			{
				IntValue = *static_cast<const int*>(Value);
			}
			case ERpVariantType::Bool:
			{
				BoolValue = *static_cast<const bool*>(Value);
			}
			default:
				break;
		}
	}

	void* Get(ERpVariantType RequestedType)
	{
		switch (RequestedType)
		{
			case ERpVariantType::Float:
			{
				return &FloatValue;
			}
			case ERpVariantType::Int:
			{
				return &IntValue;
			}
			case ERpVariantType::Bool:
			{
				return &BoolValue;
			}
			default:
				break;
		}

		return nullptr;
	}
	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ERpVariantType> Type = ERpVariantType::None;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "Type == ERpVariantType::Float", EditConditionHides = true))
	float FloatValue = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "Type == ERpVariantType::Int", EditConditionHides = true))
	int IntValue = 0;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "Type == ERpVariantType::Bool", EditConditionHides = true))
	bool BoolValue = false;
};

UENUM()
enum class EGOAPVarId : uint16
{
	Test1 = 0,
	Test2
};
 
USTRUCT()
struct FRpGOAPState
{
	GENERATED_BODY()
	
public:
	
	void AddVar(const EGOAPVarId Index, const ERpVariantType Type, const void* Value)
	{
		if (!Data.Contains(Index))
		{
			Data[Index].Set(Type, Value);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("INDEX IS ALREADY MAPPED TO A VARIABLE. Use SetVar to modify existing values."));
		}
	}

	void* GetVar(const EGOAPVarId Index, const ERpVariantType Type)
	{
		if (Data.Contains(Index))
		{
			return Data[Index].Get(Type);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO VARIABLE FOUND FOR THIS INDEX."));
		}

		return nullptr;
	}

	void SetVar(const EGOAPVarId Index, const ERpVariantType Type, const void* Value)
	{
		if (Data.Contains(Index))
		{
			Data[Index].Set(Type, Value);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO VARIABLE FOUND FOR THIS INDEX. Use AddVar to add new variables."));
		}
	}

public:
	
	UPROPERTY(EditDefaultsOnly)
	TMap<EGOAPVarId, FRpVariant> Data;
};