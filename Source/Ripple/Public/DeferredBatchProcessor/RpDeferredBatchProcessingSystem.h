// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RpBatchedCommandProcessor.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RpDeferredBatchProcessingSystem.generated.h"

USTRUCT()
struct RIPPLE_API FRpBatchProcessorConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName ProcessorName = NAME_None;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.01, UIMin = 0.01))
	float TickRate = 0.0f;
	
	UPROPERTY(EditAnywhere)
	EBatchedExecutionMode ExecutionMode = EBatchedExecutionMode::Normal;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ExecutionMode == EBatchedExecutionMode::Normal", EditConditionHides = "true", ClampMin = 1, UIMin = 1))
	uint32 BatchSize = 0;
};

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Deferred Batch Processing System")
class RIPPLE_API URpDeferredBatchProcessingSystem : public UWorldSubsystem
{
	GENERATED_BODY()

	typedef TPair<TSharedPtr<FRpBatchedCommandProcessor>, FTimerHandle> FProcessorPair;
	
public:

	virtual bool QueueCommand(FName ProcessorName, FRpCommand&& Command);
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:

	UPROPERTY(Config, EditAnywhere, Category = "Batch Processor | Configuration", meta = (TitleProperty = "ProcessorName"))
	TArray<FRpBatchProcessorConfiguration> Configurations;

	UPROPERTY(Config, EditAnywhere, Category = "Batch Processor | Configuration")
	bool bEnabled = true;

protected:
	
	TMap<FName, FProcessorPair> ProcessorMap;
	
};
