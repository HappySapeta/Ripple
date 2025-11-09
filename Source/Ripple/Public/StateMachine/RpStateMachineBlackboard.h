// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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
};
