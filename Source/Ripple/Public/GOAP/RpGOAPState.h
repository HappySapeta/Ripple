// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RpGOAPTypes.h"
#include "GameplayTagContainer.h"
#include "RpGOAPState.generated.h"

class URpGOAPAction;

UCLASS(Blueprintable, Category = "Ripple GOAP")
class URpGOAPState : public UObject
{
	GENERATED_BODY()
	
public:
	
	const URpGOAPState* Propagate(const URpGOAPAction* Action);
	const UScriptStruct* GetScriptStruct(const FGameplayTag& FactName) const;
	const FRpVariantBase* Get(const FGameplayTag& FactName) const;
	void Set(const FGameplayTag& FactName, const FRpStateDescriptor& Value);
	bool Contains(const FGameplayTag& FactName) const;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FRpStateDescriptor> Facts;
};