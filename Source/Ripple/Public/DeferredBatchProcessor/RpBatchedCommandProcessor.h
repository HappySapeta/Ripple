#pragma once

UENUM()
enum class EBatchedExecutionMode : uint8
{
	Normal,
	Stable
};

typedef TFunction<void()> FRpCommand;
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

	void Execute()
	{
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

	void Reset()
	{
		CommandQueue.Empty();
	}
	
protected:

	TArray<FRpCommand> CommandQueue;
	
	uint32 BatchSize = 0;

	EBatchedExecutionMode ExecutionMode = EBatchedExecutionMode::Normal;
};
