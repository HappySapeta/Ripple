#pragma once

// Defines the different ways of Processor execution
UENUM()
enum class EBatchedExecutionMode : uint8
{
	Normal, // Commands are executed in certain batches per timer tick. Commands are discarded after execution.
	Stable  // All commands are executed at once per timer tick. Commands are preserved after execution.
};

typedef TFunction<void()> FRpCommand;
/**
 * BatchCommandProcessor is a utility class used for breaking down large tasks into batches
 * and executing them during specific time intervals rather than executing them all in one frame.
 */
struct RIPPLE_API FRpBatchedCommandProcessor : public TSharedFromThis<FRpBatchedCommandProcessor>
{
	friend class URpDeferredBatchProcessingSystem;
public:

	FRpBatchedCommandProcessor(uint32 NewBatchSize, EBatchedExecutionMode NewExectuionMode)
		:BatchSize(NewBatchSize), ExecutionMode(NewExectuionMode)
	{}
	
	void Add(FRpCommand&& NewCommand)
	{
		CommandQueue.Push(MoveTemp(NewCommand));
	}

private:

	// Core Processor Logic
	void Execute()
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(FRpBatchedCommandProcessor::Execute)
		
		switch(ExecutionMode)
		{
		case EBatchedExecutionMode::Normal:
			{
				const uint32 NumCommands = static_cast<uint32>(CommandQueue.Num());
				uint32 Index = 0;
				while(Index < NumCommands && Index < BatchSize)
				{
					FRpCommand& Command = CommandQueue[Index];
					Command();
					++Index;
				}

				CommandQueue.RemoveAt(0, Index);
				
				break;
			}

		case EBatchedExecutionMode::Stable:
			{
				const uint32 NumCommands = static_cast<uint32>(CommandQueue.Num());
				for(uint32 Index = 0; Index < NumCommands; ++Index)
				{
					FRpCommand& Command = CommandQueue[Index];
					Command();
				}
				break;
			}
		}
	}

	// Reset Command Queue
	void Reset()
	{
		CommandQueue.Empty();
	}
	
protected:
	
	TArray<FRpCommand> CommandQueue;
	
	uint32 BatchSize = 0;

	EBatchedExecutionMode ExecutionMode = EBatchedExecutionMode::Normal;
};
