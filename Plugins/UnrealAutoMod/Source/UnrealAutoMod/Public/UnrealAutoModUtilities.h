#pragma once

#include "UnrealAutoModPrivatePCH.h"
#include "Json.h"
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Engine/DataTable.h"
#include "UnrealAutoModUtilities.generated.h"

UCLASS()
class UNREALAUTOMOD_API UUnrealAutoModUtilities : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        void LaunchExternalExecutable(const FString& FilePath, const TArray<FString>& Parameters);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        FString GetProjectDirectory();

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        TArray<FString> GetFunctionNames(UObject* Object);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        void CallFunctionByName(UObject* Object, const FString& FunctionName);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static TArray<UObject*> GetAllInstancedObjectsOfClass(UClass* Class);
};