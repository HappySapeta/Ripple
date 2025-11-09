// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RpStateMachineComponent.generated.h"

class URpState; 
class URpStateTransitionRule;

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class RIPPLE_API URpStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	URpStateMachineComponent();

	UFUNCTION(BlueprintCallable)
	void Start();
	void ProcessRules();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected: 

	UFUNCTION(BlueprintCallable)
	const URpState* GetCurrentState() const { return CurrentState; }

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URpState> StartingState;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<URpStateTransitionRule>> TransitionRules;
	
private:
	
	UPROPERTY()
	TObjectPtr<URpState> CurrentState;
};
