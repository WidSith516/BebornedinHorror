
// Copyright 2024, BebornedinHorror Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "BebornedinHorrorBPL.generated.h"

UCLASS()
class BEBORNEDINHORROR_API UBebornedinHorrorBPL : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Gets multiple rows from a Data Table and returns them as an array of structs.
     * The OutRows pin is a wildcard and will be determined by the Data Table's row struct type.
     *
     * @param Table The Data Table to get rows from.
     * @param RowNames An array of row names to retrieve.
     * @param OutRows (Output) An array of structs containing the data from the found rows.
     * @return True if the operation was successful, false otherwise.
     */
    UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "OutRows"), Category = "BebornedinHorror|DataTable")
    static bool GetDataTableRows(UDataTable* Table, TArray<FName> RowNames, TArray<FTableRowBase>& OutRows);

    // Custom Thunk declaration for the function above
    DECLARE_FUNCTION(execGetDataTableRows);
};
