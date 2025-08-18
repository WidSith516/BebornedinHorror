
// Copyright 2024, BebornedinHorror Studios. All Rights Reserved.

#include "BebornedinHorrorBPL.h"

DEFINE_FUNCTION(UBebornedinHorrorBPL::execGetDataTableRows)
{
    // Get the function parameters from the execution stack
    P_GET_OBJECT(UDataTable, Table);
    P_GET_TARRAY(FName, RowNames);

    // This is for the output array pin
    Stack.MostRecentProperty = nullptr;
    Stack.StepCompiledIn<FArrayProperty>(NULL);
    void* OutRowsAddr = Stack.MostRecentPropertyAddress;
    FArrayProperty* OutRowsProp = CastField<FArrayProperty>(Stack.MostRecentProperty);

    P_FINISH;

    bool bSuccess = false;
    if (Table && OutRowsProp)
    {
        // Get the struct type from the output array property
        FStructProperty* StructProp = CastField<FStructProperty>(OutRowsProp->Inner);
        if (StructProp)
        {            
            // Check if the Data Table's struct matches the output pin's struct type
            if (Table->GetRowStruct()->IsChildOf(StructProp->Struct))
            {
                FScriptArrayHelper OutRowsHelper(OutRowsProp, OutRowsAddr);
                OutRowsHelper.EmptyValues(); // Clear the output array first

                for (const FName& RowName : RowNames)
                {
                    uint8* RowData = Table->FindRowUnchecked(RowName);
                    if (RowData)
                    {                        
                        const int32 NewIndex = OutRowsHelper.AddValue();
                        uint8* Dest = OutRowsHelper.GetRawPtr(NewIndex);
                        StructProp->CopyCompleteValue(Dest, RowData);
                    }
                }
                bSuccess = true;
            }
            else
            {
                FFrame::KismetExecutionMessage(*FString::Printf(TEXT("GetDataTableRows error: Row struct mismatch. DataTable is %s, output pin is %s."), 
                    *Table->GetRowStruct()->GetName(), *StructProp->Struct->GetName()), ELogVerbosity::Warning);
            }
        }
    }

    // Set the return value
    *(bool*)RESULT_PARAM = bSuccess;
}
