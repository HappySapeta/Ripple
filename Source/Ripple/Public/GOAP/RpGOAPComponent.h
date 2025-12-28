// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RpGOAPComponent.generated.h"

class URpGOAPState;
class URpGOAPGoal;
class URpGOAPAction;
class URpGOAPPlanner;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RIPPLE_API URpGOAPComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	URpGOAPComponent();
	
	UFUNCTION(BlueprintCallable)
	URpGOAPPlanner* GetPlanner();
	
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Ripple GOAP")
	TSubclassOf<URpGOAPState> StartingStateClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ripple GOAP")
	TArray<TSubclassOf<URpGOAPGoal>> GoalClasses;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ripple GOAP")
	TArray<TSubclassOf<URpGOAPAction>> ActionClasses;
	
private:
	
	UPROPERTY()
	URpGOAPPlanner* Planner;
};
