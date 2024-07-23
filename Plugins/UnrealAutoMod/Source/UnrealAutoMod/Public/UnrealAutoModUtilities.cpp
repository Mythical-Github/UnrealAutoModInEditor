#include "UnrealAutoModUtilities.h"
#include "UnrealAutoModPrivatePCH.h"
#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Engine/Engine.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "UObject/UObjectIterator.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UUnrealAutoModUtilities::LaunchExternalExecutable(const FString& FilePath, const TArray<FString>& Parameters)
{
    FString Command = FString::Printf(TEXT("\"%s\""), *FilePath);

    for (const FString& Param : Parameters)
    {
        Command += FString::Printf(TEXT(" %s"), *Param);
    }

    FProcHandle ProcessHandle = FPlatformProcess::CreateProc(*FilePath, *Command, true, false, false, nullptr, 0, nullptr, nullptr);

    if (!ProcessHandle.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to launch executable: %s"), *FilePath);
    }
}

FString UUnrealAutoModUtilities::GetProjectDirectory()
{
    FString ProjectFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());
    FString ProjectDir = FPaths::GetPath(ProjectFilePath);

    return ProjectDir;
}

TArray<FString> UUnrealAutoModUtilities::GetFilesInDirectoryTree(const FString& Directory)
{
    TArray<FString> FileList;
    IFileManager& FileManager = IFileManager::Get();
    FileManager.FindFilesRecursive(FileList, *Directory, TEXT("*.*"), true, false, false);
    return FileList;
}

TArray<FString> UUnrealAutoModUtilities::GetFileExtensionsFromPaths(const TArray<FString>& FilePaths)
{
    TArray<FString> Extensions;

    for (const FString& FilePath : FilePaths)
    {
        FString Extension = FPaths::GetExtension(FilePath);
        Extensions.Add(Extension);
    }

    return Extensions;
}

bool UUnrealAutoModUtilities::CreateDirectory(const FString& Directory)
{
    return IFileManager::Get().MakeDirectory(*Directory, true);
}

bool UUnrealAutoModUtilities::DeleteDirectoryAndContents(const FString& Directory)
{
    IFileManager& FileManager = IFileManager::Get();
    FileManager.DeleteDirectory(*Directory, false, true);
    return !FileManager.DirectoryExists(*Directory);
}

bool UUnrealAutoModUtilities::DoesDirectoryExist(const FString& Directory)
{
    IFileManager& FileManager = IFileManager::Get();
    return FileManager.DirectoryExists(*Directory);
}

bool UUnrealAutoModUtilities::DoesFileExist(const FString& File)
{
    IFileManager& FileManager = IFileManager::Get();
    return FileManager.FileExists(*File);
}

bool UUnrealAutoModUtilities::CopyFile(const FString& BeforeFile, const FString& AfterFile)
{
    IFileManager& FileManager = IFileManager::Get();

    bool bSuccess = false;

    if (FileManager.FileExists(*BeforeFile))
    {
        if (FileManager.Copy(*AfterFile, *BeforeFile) == COPY_OK)
        {
            bSuccess = true;
        }
    }
    return bSuccess;
}

bool UUnrealAutoModUtilities::DeleteFile(const FString& File)
{
    IFileManager& FileManager = IFileManager::Get();

    bool bSuccess = true;

    if (FileManager.FileExists(*File))
    {
        bSuccess = FileManager.Delete(*File);

    }
    return bSuccess;
}

FString UUnrealAutoModUtilities::DataTableToJson(UDataTable* DataTable)
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

bool UUnrealAutoModUtilities::JsonToDataTable(const FString& JsonString, UDataTable* DataTable)
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

bool UUnrealAutoModUtilities::CreateTextFile(const FString& FileName, const FString& FileContents)
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (PlatformFile.FileExists(*FileName))
    {
        PlatformFile.DeleteFile(*FileName);
    }

    return FFileHelper::SaveStringToFile(FileContents, *FileName);
}

FString UUnrealAutoModUtilities::ReadFile(const FString& FileName)
{
    FString FileContents;
    if (FFileHelper::LoadFileToString(FileContents, *FileName))
    {
        return FileContents;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to read file: %s"), *FileName);
        return "";
    }
}

TArray<FString> UUnrealAutoModUtilities::GetFunctionNames(UObject* Object)
{
    TArray<FString> FunctionNames;

    if (!Object)
    {
        UE_LOG(LogTemp, Warning, TEXT("Object is null"));
        return FunctionNames;
    }

    UClass* Class = Object->GetClass();
    for (TFieldIterator<UFunction> FuncIt(Class); FuncIt; ++FuncIt)
    {
        UFunction* Function = *FuncIt;
        FunctionNames.Add(Function->GetName());
    }

    return FunctionNames;
}

void UUnrealAutoModUtilities::CallFunctionByName(UObject* Object, const FString& FunctionName)
{
    if (!Object)
    {
        UE_LOG(LogTemp, Warning, TEXT("Object is null"));
        return;
    }

    TArray<FString> AvailableFunctionNames = GetFunctionNames(Object);
    if (!AvailableFunctionNames.Contains(FunctionName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Function %s not found on object: %s"), *FunctionName, *Object->GetName());
        return;
    }

    UClass* Class = Object->GetClass();
    for (TFieldIterator<UFunction> FuncIt(Class); FuncIt; ++FuncIt)
    {
        UFunction* Function = *FuncIt;
        if (Function->GetName() == FunctionName)
        {
            FStructOnScope Params(Function);

            Object->ProcessEvent(Function, Params.GetStructMemory());
            UE_LOG(LogTemp, Log, TEXT("Called function: %s on object: %s"), *FunctionName, *Object->GetName());
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Function %s not found on object: %s"), *FunctionName, *Object->GetName());
}

TArray<UObject*> UUnrealAutoModUtilities::GetAllInstancedObjectsOfClass(UClass* Class)
{
    TArray<UObject*> Instances;

    if (!Class)
    {
        UE_LOG(LogTemp, Warning, TEXT("Class is null"));
        return Instances;
    }

    for (TObjectIterator<UObject> It; It; ++It)
    {
        UObject* Object = *It;
        if (Object && Object->IsA(Class) && !Object->IsTemplate())
        {
            Instances.Add(Object);
        }
    }

    return Instances;
}

TArray<FString> UUnrealAutoModUtilities::GetEnumValuesAsString(UEnum* Enum)
{
    TArray<FString> EnumValues;

    if (!Enum)
    {
        return EnumValues;
    }

    for (int32 i = 0; i < Enum->NumEnums(); ++i)
    {
        if (!Enum->HasMetaData(TEXT("Hidden"), i))
        {
            EnumValues.Add(Enum->GetDisplayNameText(i).ToString());
        }
    }

    return EnumValues;
}

void UUnrealAutoModUtilities::OpenDirectory(FString DirectoryPath)
{
    if (FPaths::DirectoryExists(DirectoryPath))
    {
        FPlatformProcess::ExploreFolder(*DirectoryPath);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Directory does not exist: %s"), *DirectoryPath);
    }
}

void UUnrealAutoModUtilities::OpenWebsite(FString URL)
{
    FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
}
