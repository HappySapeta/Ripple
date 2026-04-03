// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTags.h"
#include "GOAP/RpGOAPTypes.h"
#include "BehaviorTree/BlackboardData.h"
#include "RpStateMachineBlackboard.generated.h"

/**
 * RPStateMachineBlackboardBase derives from RPStateMachineBlackboard interface
 * and it is intended to be used as a base class for
 * classes implementing the RPStateMachineBlackboard interface,
 * especially in blueprints.
 * It also provides a field to store the current state of the State Machine.
 */
UCLASS(Blueprintable, BlueprintType)
class RIPPLE_API URpStateMachineBlackboardBase : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetValuesAsFloat(const FGameplayTag& Key);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetValuesAsBool(const FGameplayTag& Key);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetValuesAsInt(const FGameplayTag& Key);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector3f GetValuesAsVector3(const FGameplayTag& Key);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UObject* GetValuesAsObject(const FGameplayTag& Key);
	
	UFUNCTION(BlueprintCallable)
	void SetValuesAsFloat(const FGameplayTag& Key, float Value);
	
	UFUNCTION(BlueprintCallable)
	void SetValuesAsBool(const FGameplayTag& Key, bool Value);
	
	UFUNCTION(BlueprintCallable)
	void SetValuesAsInt(const FGameplayTag& Key, int Value);
	
	UFUNCTION(BlueprintCallable)
	void SetValuesAsVector3(const FGameplayTag& Key, FVector3f Value);
	
	UFUNCTION(BlueprintCallable)
	void SetValuesAsObject(const FGameplayTag& Key, UObject* Value);
	
private: 
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FRpStateDescriptor> Facts;
};

inline float URpStateMachineBlackboardBase::GetValuesAsFloat(const FGameplayTag& Key)
{
	if (ensureAlwaysMsgf(Facts.Contains(Key), TEXT("Fact table doesn't contain key %s"), *Key.ToString()))
	{
		const float Value = Facts[Key].Fact.GetPtr<FRpVariantFloat>()->Value;
		return Value;
	}
	return 0.0f;
}

inline bool URpStateMachineBlackboardBase::GetValuesAsBool(const FGameplayTag& Key)
{
	if (ensureAlwaysMsgf(Facts.Contains(Key), TEXT("Fact table doesn't contain key %s"), *Key.ToString()))
	{
		const bool Value = Facts[Key].Fact.GetPtr<FRpVariantBool>()->Value;
		return Value;
	}
	return false;
}

inline int URpStateMachineBlackboardBase::GetValuesAsInt(const FGameplayTag& Key)
{
	if (ensureAlwaysMsgf(Facts.Contains(Key), TEXT("Fact table doesn't contain key %s"), *Key.ToString()))
	{
		const int Value = Facts[Key].Fact.GetPtr<FRpVariantInteger>()->Value;
		return Value;
	}
	return 0;
}

inline FVector3f URpStateMachineBlackboardBase::GetValuesAsVector3(const FGameplayTag& Key)
{
	if (ensureAlwaysMsgf(Facts.Contains(Key), TEXT("Fact table doesn't contain key %s"), *Key.ToString()))
	{
		const FVector3f Value = Facts[Key].Fact.GetPtr<FRpVariantVector3>()->Value;
		return Value;
	}
	return {0,0,0};
}

inline UObject* URpStateMachineBlackboardBase::GetValuesAsObject(const FGameplayTag& Key)
{
	if (ensureAlwaysMsgf(Facts.Contains(Key), TEXT("Fact table doesn't contain key %s"), *Key.ToString()))
	{
		UObject* Value = Facts[Key].Fact.GetPtr<FRpVariantObject>()->Value;
		return Value;
	}
	return nullptr;
}

inline void URpStateMachineBlackboardBase::SetValuesAsFloat(const FGameplayTag& Key, float Value)
{
	if (ensureAlwaysMsgf(Facts.Contains(Key), TEXT("Fact table doesn't contain key %s"), *Key.ToString()))
	{
		auto& Fact = Facts[Key].Fact;
		if (Fact.IsValid())
		{
			Fact.GetMutable<FRpVariantFloat>().Value = Value;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fact data is invalid."))
		}
	}
}

inline void URpStateMachineBlackboardBase::SetValuesAsBool(const FGameplayTag& Key, bool Value)
{
	if (ensureAlwaysMsgf(Facts.Contains(Key), TEXT("Fact table doesn't contain key %s"), *Key.ToString()))
	{
		auto& Fact = Facts[Key].Fact;
		if (Fact.IsValid())
		{
			Fact.GetMutable<FRpVariantBool>().Value = Value;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fact data is invalid."))
		}
	}
}

inline void URpStateMachineBlackboardBase::SetValuesAsInt(const FGameplayTag& Key, int Value)
{
	if (ensureAlwaysMsgf(Facts.Contains(Key), TEXT("Fact table doesn't contain key %s"), *Key.ToString()))
	{
		auto& Fact = Facts[Key].Fact;
		if (Fact.IsValid())
		{
			Fact.GetMutable<FRpVariantInteger>().Value = Value;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fact data is invalid."))
		}
	}
}

inline void URpStateMachineBlackboardBase::SetValuesAsVector3(const FGameplayTag& Key, FVector3f Value)
{
	if (ensureAlwaysMsgf(Facts.Contains(Key), TEXT("Fact table doesn't contain key %s"), *Key.ToString()))
	{
		auto& Fact = Facts[Key].Fact;
		if (Fact.IsValid())
		{
			Fact.GetMutable<FRpVariantVector3>().Value = Value;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fact data is invalid."))
		}
	}
}

inline void URpStateMachineBlackboardBase::SetValuesAsObject(const FGameplayTag& Key, UObject* Value)
{
	if (ensureAlwaysMsgf(Facts.Contains(Key), TEXT("Fact table doesn't contain key %s"), *Key.ToString()))
	{
		auto& Fact = Facts[Key].Fact;
		if (Fact.IsValid())
		{
			Fact.GetMutable<FRpVariantObject>().Value = Value;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fact data is invalid."))
		}
	}
}
