// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RpStateMachineComponent.generated.h"

class URpStateMachineBlackboardBase;
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

public:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URpState> StartingState;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<URpState>> StateClasses;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<URpStateTransitionRule>> TransitionRules;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URpStateMachineBlackboardBase> StatemachineBBClass;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<URpStateMachineBlackboardBase> StateMachineBlackboard;
	
private:
	
	UPROPERTY()
	TObjectPtr<URpState> CurrentState;
	
	UPROPERTY(Transient)
	TArray<URpState*> StateInstances;
	
	UPROPERTY(Transient)
	TArray<URpStateTransitionRule*> TransitionRuleInstances;
};
