// Copyright Anupam Sahu. All Rights Reserved.

#include "GOAP/RpGOAPState.h"
#include "GOAP/RpGOAPGoal.h"

bool URpGOAPState::DoesSatisfyRequirements(const TMap<FGameplayTag, FRpRequirementDescriptor>& Requirements) const
{
	for (const auto& [FactName, Requirement] : Requirements)
	{
		if (!Facts.Contains(FactName))
		{
			return false;
		}

		switch (Requirement.Condition)
		{
			case EQUAL:
			{
				const bool bResult = Requirement.Fact.GetPtr<FRpVariantBase>()->operator==(GetFact(FactName));
				if (bResult == false)
				{
					return false;
				}
				break;
			}
			case LESS:
			{
				const bool bResult = Requirement.Fact.GetPtr<FRpVariantBase>()->operator>(GetFact(FactName));
				if (bResult == false)
				{
					return false;
				}
				break;
			}
			case GREATER:
			{
				const bool bResult = Requirement.Fact.GetPtr<FRpVariantBase>()->operator<(GetFact(FactName));
				if (bResult == false)
				{
					return false;
				}
				break;
			}
			default:
				break;
		}
	}
	return true;	
}

bool URpGOAPState::WillHaveEffects(const TMap<FGameplayTag, FRpStateDescriptor>& Effects)
{
	for (const auto& [FactName, Effect] : Effects)
	{
		if (Effect.Fact.GetPtr<FRpVariantBase>()->operator!=(GetFact(FactName)))
		{
			return true;
		}
	}
	
	return false;
}

int URpGOAPState::CalcDistanceFromState(const URpGOAPState* State)
{
	int Distance = 0; 
	for (const auto& [FactName, Requirement] : State->Facts)
	{
		if (!Facts.Contains(FactName)) // This is never supposed to happen. Very bad if it did.
		{
			UE_LOG(LogTemp, Error, TEXT("Fact not found!"));
			return TNumericLimits<int>::Max();
		}
		
		Distance += Requirement.Fact.GetPtr<FRpVariantBase>()->GetAbsDifference(GetFact(FactName));
	}
	return Distance;
}

int URpGOAPState::CalcDistanceFromGoal(const URpGOAPGoal* Goal)
{
	int Distance = 0; 
	for (const auto& [FactName, Requirement] : Goal->GetRequirements())
	{
		if (!Facts.Contains(FactName)) // This is never supposed to happen. Very bad if it did.
		{
			UE_LOG(LogTemp, Error, TEXT("Fact not found!"));
			return TNumericLimits<int>::Max();
		}
		
		Distance += Requirement.Fact.GetPtr<FRpVariantBase>()->GetAbsDifference(GetFact(FactName));
	}
	return Distance;
}

bool URpGOAPState::operator==(const URpGOAPState& Other) const
{
	if (Facts.Num() != Other.Facts.Num())
	{
		return false;
	}
	
	for (const auto& [FactName, FactDescriptor] : Other.Facts)
	{
		if (FactDescriptor.Fact.GetPtr<FRpVariantBase>()->operator==(GetFact(FactName)))
		{
			continue;
		}
		
		return false;
	}
	
	return true;
}

const UScriptStruct* URpGOAPState::GetScriptStruct(const FGameplayTag& FactName) const
{
	if (Facts.Contains(FactName))
	{
		return Facts[FactName].Fact.GetScriptStruct();
	}
	
	return nullptr;
}

const FRpVariantBase* URpGOAPState::GetFact(const FGameplayTag& FactName) const
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

bool URpGOAPState::SetFact(const FGameplayTag& FactName, const FRpStateDescriptor& Value)
{
	if (!Facts.Contains(FactName))
	{
		return false;
	}
	
	if (ensureAlwaysMsgf(GetScriptStruct(FactName) == Value.Fact.GetScriptStruct(),TEXT("ScriptStructs do not match")))
	{
		Facts[FactName] = Value;
		return true;
	}
	
	return false;
}
