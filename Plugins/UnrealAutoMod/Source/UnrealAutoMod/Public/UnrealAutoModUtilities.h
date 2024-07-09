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
        TArray<FString> GetFilesInDirectoryTree(const FString& Directory);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        TArray<FString> GetFileExtensionsFromPaths(const TArray<FString>& FilePaths);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        bool CreateDirectory(const FString& Directory);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        bool DeleteDirectoryAndContents(const FString& Directory);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        bool DoesDirectoryExist(const FString& Directory);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        bool DoesFileExist(const FString& File);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        bool CopyFile(const FString& BeforeFile, const FString& AfterFile);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        bool DeleteFile(const FString& File);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        FString DataTableToJson(UDataTable* DataTable);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        bool JsonToDataTable(const FString& JsonString, UDataTable* DataTable);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        bool CreateTextFile(const FString& FileName, const FString& FileContents);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        FString ReadFile(const FString& FileName);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        TArray<FString> GetFunctionNames(UObject* Object);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        void CallFunctionByName(UObject* Object, const FString& FunctionName);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static TArray<UObject*> GetAllInstancedObjectsOfClass(UClass* Class);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static TArray<FString> GetEnumValuesAsString(UEnum* Enum);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static void OpenDirectory(FString DirectoryPath);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static void OpenWebsite(FString URL);
};