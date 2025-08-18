
// Copyright 2024, BebornedinHorror Studios. All Rights Reserved.

#include "AsyncLoadDataTable.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

UAsyncLoadDataTable* UAsyncLoadDataTable::AsyncLoadDataTable(UObject* WorldContextObject, TSoftObjectPtr<UDataTable> DataTableToLoad)
{
    // Create a new instance of our action
    UAsyncLoadDataTable* NewAction = NewObject<UAsyncLoadDataTable>();
    NewAction->DataTableToLoad = DataTableToLoad;
    NewAction->RegisterWithGameInstance(WorldContextObject);
    return NewAction;
}

void UAsyncLoadDataTable::Activate()
{
    if (!DataTableToLoad.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("AsyncLoadDataTable: DataTableToLoad is not a valid soft pointer."));
        OnFailure.Broadcast(nullptr);
        SetReadyToDestroy();
        return;
    }

    // Check if the asset is already loaded
    if (DataTableToLoad.Get())
    {
        OnSuccess.Broadcast(DataTableToLoad.Get());
        SetReadyToDestroy();
        return;
    }

    // If not loaded, request an asynchronous load
    FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
    StreamableManager.RequestAsyncLoad(DataTableToLoad.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UAsyncLoadDataTable::OnDataTableLoaded));
}

void UAsyncLoadDataTable::OnDataTableLoaded()
{
    UDataTable* LoadedTable = Cast<UDataTable>(DataTableToLoad.Get());
    if (LoadedTable)
    {
        OnSuccess.Broadcast(LoadedTable);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AsyncLoadDataTable: Failed to load Data Table asset: %s"), *DataTableToLoad.ToString());
        OnFailure.Broadcast(nullptr);
    }

    // The action is complete, mark it for destruction
    SetReadyToDestroy();
}
