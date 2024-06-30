#pragma once

#include "UnrealAutoModPrivatePCH.h"
#include "Blueprint/UserWidget.h"
#include "Json.h"
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Engine/DataTable.h"
#include "UnrealAutoModBaseWidget.generated.h"

UCLASS()
class UNREALAUTOMOD_API UUnrealAutoModBaseWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "JSON")
        FString DataTableToJson(UDataTable* DataTable);

    UFUNCTION(BlueprintCallable, Category = "JSON")
        bool JsonToDataTable(const FString& JsonString, UDataTable* DataTable);
};
