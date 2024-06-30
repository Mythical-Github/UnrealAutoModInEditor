#include "UnrealAutoModPrivatePCH.h"
#include "UnrealAutoModUtilities.h"
#include "PlatformProcess.h"
#include "Paths.h"
#include "HAL/FileManager.h"
#include "Engine/Engine.h"

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
