#pragma once

#include "UnrealAutoModPrivatePCH.h"
#include "Blueprint/UserWidget.h"
#include "UObject/UObject.h" // Include for UObject
#include "UObject/Class.h"   // Include for UClass

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

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        UObject* LoadObjectByPath(const FString& ObjectPath);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        bool IsValidObject(UObject* Object);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        bool IsValidClass(UClass* Class);
};
