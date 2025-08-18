
// Copyright 2024, BebornedinHorror Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Engine/DataTable.h"
#include "AsyncLoadDataTable.generated.h"

// Delegate for when the Data Table is successfully loaded or fails to load.
// The Blueprint node will have two output execution pins: OnSuccess and OnFailure.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncLoadDataTablePin, UDataTable*, LoadedDataTable);

/**
 * Asynchronously loads a UDataTable.
 * This allows you to load a data table in the background without blocking the game thread.
 */
UCLASS()
class BEBORNEDINHORROR_API UAsyncLoadDataTable : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    // Output delegate pins
    UPROPERTY(BlueprintAssignable)
    FAsyncLoadDataTablePin OnSuccess;

    UPROPERTY(BlueprintAssignable)
    FAsyncLoadDataTablePin OnFailure;

    /**
     * Asynchronously loads a Data Table asset.
     * @param WorldContextObject The world context.
     * @param DataTableToLoad A soft pointer to the Data Table asset to load.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "BebornedinHorror|Async")
    static UAsyncLoadDataTable* AsyncLoadDataTable(UObject* WorldContextObject, TSoftObjectPtr<UDataTable> DataTableToLoad);

    // UBlueprintAsyncActionBase interface
    virtual void Activate() override;

private:
    // The Data Table asset we want to load
    TSoftObjectPtr<UDataTable> DataTableToLoad;

    // Internal callback function when loading is complete
    void OnDataTableLoaded();
};
