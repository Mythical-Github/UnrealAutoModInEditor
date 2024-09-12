#pragma once

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
		static void LaunchExternalExecutable(const FString& FilePath, const TArray<FString>& Parameters);

	UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static void LaunchExternalExecutableNoHead(const FString& FilePath, const TArray<FString>& Parameters);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static FString GetProjectDirectory();

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static TArray<FString> GetFilesInDirectoryTree(const FString& Directory);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static TArray<FString> GetFileExtensionsFromPaths(const TArray<FString>& FilePaths);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static bool CreateDirectory(const FString& Directory);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static bool DeleteDirectoryAndContents(const FString& Directory);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static bool DoesDirectoryExist(const FString& Directory);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static bool DoesFileExist(const FString& File);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static bool CopyFile(const FString& BeforeFile, const FString& AfterFile);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static bool DeleteFile(const FString& File);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static FString DataTableToJson(UDataTable* DataTable);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static bool JsonToDataTable(const FString& JsonString, UDataTable* DataTable);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static bool CreateTextFile(const FString& FileName, const FString& FileContents);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static FString ReadFile(const FString& FileName);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static TArray<FString> GetFunctionNames(UObject* Object);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static void CallFunctionByName(UObject* Object, const FString& FunctionName);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static TArray<UObject*> GetAllInstancedObjectsOfClass(UClass* Class);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static TArray<FString> GetEnumValuesAsString(UEnum* Enum);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static void OpenDirectory(FString DirectoryPath);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static void OpenWebsite(FString URL);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static bool SetJsonFieldAsString(const FString& JsonString, const TArray<FString>& FieldNames, const FString& FieldValue, FString& OutJsonString);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static bool GetJsonFieldAsString(const FString& JsonString, const TArray<FString>& FieldNames, FString& FieldValue);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static bool WriteStringToFile(const FString& FileName, const FString& FileContents);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static bool RemoveJsonField(const FString& JsonString, const TArray<FString>& FieldNames, FString& OutJsonString);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static bool ReadJsonField(const FString& JsonString, const TArray<FString>& FieldNames, FString& FieldValue);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static bool AddJsonListEntry(const FString& JsonString, const TArray<FString>& FieldNames, const FString& EntryValue, FString& OutJsonString);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static bool RemoveJsonListEntry(const FString& JsonString, const TArray<FString>& FieldNames, int32 Index, FString& OutJsonString);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static bool GetJsonArrayEntries(const FString& JsonString, const TArray<FString>& FieldNames, TArray<FString>& Entries);

    UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
        static bool CheckIfJsonFieldExists(const FString& JsonString, const TArray<FString>& FieldNames, bool& Exists);

	UFUNCTION(BlueprintCallable, Category = "UnrealAutoMod")
		static FString GetParentDirectory(const FString& FilePath);
};