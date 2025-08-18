
// Copyright 2024, BebornedinHorror Studios. All Rights Reserved.

#include "AsyncExecFunction.h"

UAsyncExecFunction* UAsyncExecFunction::ExecuteFunctionAsync(UObject* WorldContextObject, const FString& InString, float InExecutionTime)
{
    UAsyncExecFunction* NewAction = NewObject<UAsyncExecFunction>();
    NewAction->InputString = InString;
    NewAction->ExecutionTime = FMath::Max(0.0f, InExecutionTime); // Ensure non-negative time
    NewAction->RegisterWithGameInstance(WorldContextObject);
    return NewAction;
}

void UAsyncExecFunction::Activate()
{
    // Create the callback function
    TFunction<void(const FString&)> Callback = [this](const FString& Result)
    {
        this->OnTaskCompleted(Result);
    };

    // Create and start the background task
    Task = new FMyGenericAsyncTask(InputString, ExecutionTime, Callback);

    if (Task)
    {
        // Use FAutoDeleteAsyncTask to automatically delete the task object when it's done.
        (new FAutoDeleteAsyncTask<FMyGenericAsyncTask>(InputString, ExecutionTime, Callback))->StartBackgroundTask();
    }
    else
    {
        OnFailure.Broadcast("Failed to create async task.");
        SetReadyToDestroy();
    }
}

void UAsyncExecFunction::OnTaskCompleted(const FString& Result)
{
    if (Result.IsEmpty())
    {
        OnFailure.Broadcast("Task returned an empty result.");
    }
    else
    {
        OnSuccess.Broadcast(Result);
    }
    SetReadyToDestroy();
}
