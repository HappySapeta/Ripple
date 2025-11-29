// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RpGOAPComponent.generated.h"

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

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Ripple GOAP")
	TSubclassOf<URpGOAPPlanner> PlannerClass;
};
