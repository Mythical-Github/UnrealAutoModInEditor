#pragma once

#include "UnrealAutoModPrivatePCH.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h" // Include for launching processes
#include "Runtime/Core/Public/Misc/Paths.h" // Include for file path utilities

#include "UnrealAutoModBaseWidget.generated.h"

/**
 *
 */
UCLASS()
class UNREALAUTOMOD_API UUnrealAutoModBaseWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Function declaration to launch an external executable with an array of parameters
    UFUNCTION(BlueprintCallable, Category = "Utility")
        void LaunchExternalExecutable(const FString& FilePath, const TArray<FString>& Parameters);

    // Function declaration to get the directory of the .uproject file
    UFUNCTION(BlueprintCallable, Category = "Utility")
        FString GetProjectDirectory();
};
