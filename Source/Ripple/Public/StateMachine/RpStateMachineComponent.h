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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, FString, NewState);
	
public:

	URpStateMachineComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ProcessRules();
	
	UFUNCTION(BlueprintCallable)
	void Start();

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ContextSubClass"))
	URpStateMachineBlackboardBase* GetContext(TSubclassOf<URpStateMachineBlackboardBase> ContextSubClass)
	{
		return StateMachineBlackboard;
	}
	
protected: 

	UFUNCTION(BlueprintCallable)
	const URpState* GetCurrentState() const { return CurrentState; }

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnStateChanged OnStateChanged;

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
