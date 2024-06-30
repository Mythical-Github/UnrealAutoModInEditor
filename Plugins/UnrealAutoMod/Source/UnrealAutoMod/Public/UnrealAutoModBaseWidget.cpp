#include "UnrealAutoModPrivatePCH.h"
#include "UnrealAutoModBaseWidget.h"

FString UUnrealAutoModBaseWidget::DataTableToJson(UDataTable* DataTable)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null"));
        return "";
    }

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);

    if (DataTable->WriteTableAsJSON(Writer))
    {
        Writer->Close();
        return OutputString;
    }

    return "";
}

bool UUnrealAutoModBaseWidget::JsonToDataTable(const FString& JsonString, UDataTable* DataTable)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null"));
        return false;
    }

    TArray<FString> Problems = DataTable->CreateTableFromJSONString(JsonString);
    if (Problems.Num() > 0)
    {
        for (const FString& Problem : Problems)
        {
            UE_LOG(LogTemp, Warning, TEXT("Problem: %s"), *Problem);
        }
        return false;
    }

    return true;
}

