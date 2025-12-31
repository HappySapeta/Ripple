// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RpGOAPTypes.h"
#include "GameplayTagContainer.h"
#include "RpGOAPState.generated.h"

class URpGOAPAction;

USTRUCT(NotBlueprintable, NotBlueprintType)
struct FRpGOAPAStarNode
{
	GENERATED_BODY()
	
public:
	
	void Reset()
	{
		GCost = BIG_NUMBER;
		HCost = BIG_NUMBER;
		LinkingAction = nullptr;
		bSeen = false;
	}
	
	float GetFCost() const{return GCost + HCost;}
	void SetGCost(const int Value){GCost = Value;}
	float GetGCost() const{return GCost;}
	void SetHCost(const int Value){HCost = Value;}
	float GetHCost() const{return HCost;}
	
	void SetLinkingAction(const URpGOAPAction* Action){LinkingAction = Action;}
	const URpGOAPAction* GetLinkingAction() const{return LinkingAction;}

	void SetSeen(bool Value)
	{
		bSeen = Value;
	}
	
	bool IsSeen() const
	{
		return bSeen;
	}

	void SetParent(URpGOAPState* Current)
	{
		Parent = Current;
	}
	
	URpGOAPState* GetParent() const
	{
		return Parent;
	}

private:
	
	float GCost = BIG_NUMBER;
	float HCost = BIG_NUMBER;
	bool bSeen = false;
	
	UPROPERTY()
	const URpGOAPAction* LinkingAction = nullptr;
	
	UPROPERTY()
	URpGOAPState* Parent = nullptr;
};

UCLASS(Blueprintable, Category = "Ripple GOAP")
class URpGOAPState : public UObject
{
	GENERATED_BODY()
	
public:
	
	URpGOAPState()
	{
		AStarNode.Reset();
	}

	UFUNCTION(BlueprintCallable)
	const UScriptStruct* GetScriptStruct(const FGameplayTag& FactName) const;
	
	UFUNCTION(BlueprintCallable)
	bool SetFact(const FGameplayTag& FactName, const FRpStateDescriptor& Value);
	
	const FRpVariantBase* GetFact(const FGameplayTag& FactName) const;
	
	bool DoesSatisfyRequirements(const TMap<FGameplayTag, FRpRequirementDescriptor>& Requirements) const;
	bool WillHaveEffects(const TMap<FGameplayTag, FRpStateDescriptor>& Effects);
	int CalcDistanceFromState(const URpGOAPState* State);
	int CalcDistanceFromGoal(const URpGOAPGoal* Goal);
	
	FRpGOAPAStarNode& GetAStarNode()
	{
		return AStarNode;
	}
	
	bool operator==(const URpGOAPState& Other) const;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FRpStateDescriptor> Facts;

private:
	
	FRpGOAPAStarNode AStarNode;
};