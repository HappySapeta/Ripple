// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RpGOAPState.h"
#include "UObject/Object.h"
#include "RpGOAPGoal.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class RIPPLE_API URpGOAPGoal : public UObject
{
	GENERATED_BODY()

public:
	
protected:

	UPROPERTY(EditAnywhere, DisplayName = "RequiredState")
	FRpGOAPState State;
};
