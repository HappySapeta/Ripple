// Copyright Anupam Sahu. All Rights Reserved.

#include "DeferredBatchProcessor/RpDeferredBatchProcessingSystem.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

void URpDeferredBatchProcessingSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	const UWorld* World = GetWorld();
	if(ensureAlways(World))
	{
		for(const FRpBatchProcessorConfiguration& Configuration : Configurations)
		{
			const FName& Name = Configuration.ProcessorName;
			if(!ProcessorMap.Contains(Name))
			{
				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate;

				FProcessorPair ProcessorPair;

				ProcessorPair.Get<0>() = MakeShared<FRpBatchedCommandProcessor>(Configuration.BatchSize, Configuration.ExecutionMode);
				TimerDelegate.BindSP(ProcessorPair.Get<0>().Get(), &FRpBatchedCommandProcessor::Execute);

				World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Configuration.TickRate, true);
				ProcessorPair.Get<1>() = TimerHandle;
				
				ProcessorMap.Add(Name, ProcessorPair);
			}
		}
	}
	
	Super::Initialize(Collection);
}

bool URpDeferredBatchProcessingSystem::QueueCommand(FName ProcessorName, FRpCommand&& Command)
{
	if(ensureMsgf(ProcessorMap.Contains(ProcessorName), TEXT("Processor %s doesn't exist"), *ProcessorName.ToString()))
	{
		ProcessorMap[ProcessorName].Get<0>()->Add(MoveTemp(Command));
		return true;
	}

	return false;
}

bool URpDeferredBatchProcessingSystem::ShouldCreateSubsystem(UObject* Outer) const
{
#if WITH_EDITOR
	return (GEditor && GEditor->IsPlaySessionInProgress());
#endif
	return bEnabled;
}
