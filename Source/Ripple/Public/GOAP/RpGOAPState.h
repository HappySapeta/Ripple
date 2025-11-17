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
	
	bool operator==(const FRpVariant& Other) const
	{
		if (Other.Type != Type)
		{
			return false;
		}
		
		switch (Type)
		{
			case ERpVariantType::Float:
			{
				return FloatValue == Other.FloatValue;
			}
			case ERpVariantType::Int:
			{
				return IntValue == Other.IntValue;
			}
			case ERpVariantType::Bool:
			{
				return BoolValue == Other.BoolValue;
			}
			default:
				break;
		}
		
		return false;
	}
	
	static bool IsNearlyEqual(const FRpVariant& Lhs, const FRpVariant& Rhs, const float Tolerance = 0.00001f)
	{
		if (Lhs.Type != Rhs.Type)
		{
			return false;
		}
		
		switch (Lhs.Type)
		{
			case ERpVariantType::Float:
			{
				return (Lhs.FloatValue - Rhs.FloatValue) < Tolerance;
			}
			case ERpVariantType::Int:
			{
				return (Lhs.IntValue - Rhs.IntValue) < Tolerance;
			}
			case ERpVariantType::Bool:
			{
				return Lhs.BoolValue == Rhs.BoolValue;
			}
			default:
				break;
		}
		
		return false;
	}

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
enum class EGOAPFactName : uint16
{
	Test1 = 0,
	Test2
};
 
USTRUCT()
struct FRpGOAPState
{
	GENERATED_BODY()
	
public:
	
	void AddVar(const EGOAPFactName Index, const ERpVariantType Type, const void* Value)
	{
		if (!Facts.Contains(Index))
		{
			Facts[Index].Set(Type, Value);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("INDEX IS ALREADY MAPPED TO A VARIABLE. Use SetVar to modify existing values."));
		}
	}

	void* GetVar(const EGOAPFactName Index, const ERpVariantType Type)
	{
		if (Facts.Contains(Index))
		{
			return Facts[Index].Get(Type);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO VARIABLE FOUND FOR THIS INDEX."));
		}

		return nullptr;
	}

	void SetVar(const EGOAPFactName Index, const ERpVariantType Type, const void* Value)
	{
		if (Facts.Contains(Index))
		{
			Facts[Index].Set(Type, Value);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO VARIABLE FOUND FOR THIS INDEX. Use AddVar to add new variables."));
		}
	}

public:
	
	UPROPERTY(EditDefaultsOnly)
	TMap<EGOAPFactName, FRpVariant> Facts;
};