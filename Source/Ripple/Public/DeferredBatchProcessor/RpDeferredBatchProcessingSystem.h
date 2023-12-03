// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RpBatchedCommandProcessor.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RpDeferredBatchProcessingSystem.generated.h"

/**
 * Settings to configure the Batch Processor.
 * Each process has its own Configuration.
 */
USTRUCT()
struct RIPPLE_API FRpBatchProcessorConfiguration
{
	GENERATED_BODY()

	// Unique name to identify Processors
	UPROPERTY(EditAnywhere)
	FName ProcessorName = NAME_None;

	// Defines the rate at which the Processor's logic is execute
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.01, UIMin = 0.01))
	float TickRate = 0.0f;

	// Processor's Execution Mode
	UPROPERTY(EditAnywhere)
	EBatchedExecutionMode ExecutionMode = EBatchedExecutionMode::Normal;

	// Batch Size
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

	// Add a Command to the Queue.
	virtual bool QueueCommand(FName ProcessorName, FRpCommand&& Command);

	// Should create if in Editor and in PIE and if Enabled.
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	// Read Processor Configurations and set Timers.
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:

	UPROPERTY(Config, EditAnywhere, Category = "Batch Processor | Configuration", meta = (TitleProperty = "ProcessorName"))
	TArray<FRpBatchProcessorConfiguration> Configurations;

	UPROPERTY(Config, EditAnywhere, Category = "Batch Processor | Configuration")
	bool bEnabled = true;

protected:
	
	TMap<FName, FProcessorPair> ProcessorMap;
	
};
