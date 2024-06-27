#pragma once

#include "UnrealAutoModPrivatePCH.h"
#include "Blueprint/UserWidget.h"
#include "UObject/UObject.h"
#include "UObject/Class.h"
#include "Engine/DataTable.h"

#include "UnrealAutoModBaseWidget.generated.h"

/**
 *
 */
UCLASS()
class UNREALAUTOMOD_API UUnrealAutoModBaseWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        void LaunchExternalExecutable(const FString& FilePath, const TArray<FString>& Parameters);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        FString GetProjectDirectory();
};
