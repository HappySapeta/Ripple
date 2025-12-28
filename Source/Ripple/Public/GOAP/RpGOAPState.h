// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RpGOAPTypes.h"
#include "GameplayTagContainer.h"
#include "RpGOAPState.generated.h"

class URpGOAPAction;

USTRUCT(NotBlueprintable, NotBlueprintType)
struct FMyStruct
{
	GENERATED_BODY()
	
	int GCost = 0;
	int HCost = 0;
	
	UPROPERTY()
	const URpGOAPAction* LinkingAction;
};

UCLASS(Blueprintable, Category = "Ripple GOAP")
class URpGOAPState : public UObject
{
	GENERATED_BODY()
	
public:
	
	const UScriptStruct* GetScriptStruct(const FGameplayTag& FactName) const;
	const FRpVariantBase* GetFact(const FGameplayTag& FactName) const;
	void SetFact(const FGameplayTag& FactName, const FRpStateDescriptor& Value);
	
	bool DoesSatisfyRequirements(const TMap<FGameplayTag, FRpRequirementDescriptor>& Requirements) const;
	int CalcDistanceFromState(const URpGOAPState* State);
	int CalcDistanceFromGoal(const URpGOAPGoal* Goal);
	
	int GetFCost() const{return GCost + HCost;}
	void SetGCost(const int Value){GCost = Value;}
	int GetGCost() const{return GCost;}
	void SetHCost(const int Value){HCost = Value;}
	int GetHCost() const{return HCost;}
	
	void SetLinkingAction(const URpGOAPAction* Action){LinkingAction = Action;}
	const URpGOAPAction* GetLinkingAction() const{return LinkingAction;}
	bool operator==(const URpGOAPState& Other) const;

	void SetSeen(bool Value)
	{
		bSeen = Value;
	}
	
	bool IsSeen() const
	{
		return bSeen;
	}

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FRpStateDescriptor> Facts;

private:
	
	int GCost = 0;
	int HCost = 0;
	bool bSeen = false;
	
	UPROPERTY()
	const URpGOAPAction* LinkingAction;
};