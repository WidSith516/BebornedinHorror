
// Copyright 2024, BebornedinHorror Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Async/AsyncWork.h"
#include "AsyncExecFunction.generated.h"

// Forward declaration for the task class
class FMyGenericAsyncTask;

// Delegate for when the async function completes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncExecPin, const FString&, ResultString);

/**
 * A Blueprint node to execute a function on a background thread.
 */
UCLASS()
class BEBORNEDINHORROR_API UAsyncExecFunction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncExecPin OnSuccess;

    UPROPERTY(BlueprintAssignable)
    FAsyncExecPin OnFailure;

    /**
     * Executes a long-running function on a background thread.
     * @param InString An example input string for the task.
     * @param InExecutionTime How long the task should "simulate" work in seconds.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "BebornedinHorror|Async")
    static UAsyncExecFunction* ExecuteFunctionAsync(UObject* WorldContextObject, const FString& InString, float InExecutionTime);

    virtual void Activate() override;

private:
    // The task instance
    FMyGenericAsyncTask* Task;

    // Inputs for the task
    FString InputString;
    float ExecutionTime;

    // Callback function when the task is complete
    void OnTaskCompleted(const FString& Result);
};


/**
 * The actual task that runs on a background thread.
 * IMPORTANT: You cannot interact with UObjects (Actors, Components, etc.) inside DoWork().
 */
class FMyGenericAsyncTask : public FNonAbandonableTask
{
public:
    FMyGenericAsyncTask(const FString& InString, float InExecutionTime, TFunction<void(const FString&)> InCallback)
        : InputString(InString), ExecutionTime(InExecutionTime), OnCompleteCallback(InCallback)
    {}

    // This is the method that gets executed on the background thread.
    void DoWork()
    {
        // --- Simulate a long-running task ---
        FPlatformProcess::Sleep(ExecutionTime);

        // --- Perform your actual work here ---
        // For example, complex calculations, data processing, etc.
        // IMPORTANT: Do NOT access or modify UObjects here.
        const FString Result = FString::Printf(TEXT("Task completed for input: '%s' after %.2f seconds."), *InputString, ExecutionTime);

        // --- When done, schedule the callback to run on the Game Thread ---
        AsyncTask(ENamedThreads::GameThread, [this, Result]()
        {
            OnCompleteCallback(Result);
        });
    }

    // Required for FNonAbandonableTask
    FORCEINLINE TStatId GetStatId() const
    {
        RETURN_QUICK_DECLARE_CYCLE_STAT(FMyGenericAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
    }

private:
    FString InputString;
    float ExecutionTime;
    TFunction<void(const FString&)> OnCompleteCallback;
};
