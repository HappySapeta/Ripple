// Copyright Anupam Sahu. All Rights Reserved.

#include "GOAP/RpGOAPState.h"

const URpGOAPState* URpGOAPState::Propagate(const URpGOAPAction* Action)
{
	return nullptr;
}

bool URpGOAPState::Contains(const FGameplayTag& FactName) const
{
	return Facts.Contains(FactName);
}

const UScriptStruct* URpGOAPState::GetScriptStruct(const FGameplayTag& FactName) const
{
	if (Facts.Contains(FactName))
	{
		return Facts[FactName].Fact.GetScriptStruct();
	}
	
	return nullptr;
}

const FRpVariantBase* URpGOAPState::Get(const FGameplayTag& FactName) const
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

void URpGOAPState::Set(const FGameplayTag& FactName, const FRpStateDescriptor& Value)
{
	if (ensureAlwaysMsgf(GetScriptStruct(FactName) == Value.Fact.GetScriptStruct(),TEXT("ScriptStructs do not match")))
	{
		Facts[FactName] = Value;
	}
}
