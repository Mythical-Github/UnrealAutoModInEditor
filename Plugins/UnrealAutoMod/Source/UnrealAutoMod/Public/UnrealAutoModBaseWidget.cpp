#include "UnrealAutoModPrivatePCH.h"
#include "UnrealAutoModBaseWidget.h"
#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Engine/Engine.h"

void UUnrealAutoModBaseWidget::LaunchExternalExecutable(const FString& FilePath, const TArray<FString>& Parameters)
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

FString UUnrealAutoModBaseWidget::GetProjectDirectory()
{
    FString ProjectFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());
    FString ProjectDir = FPaths::GetPath(ProjectFilePath);

    return ProjectDir;
}

TArray<FString> UUnrealAutoModBaseWidget::GetFilesInDirectoryTree(const FString& Directory)
{
    TArray<FString> FileList;
    IFileManager& FileManager = IFileManager::Get();
    FileManager.FindFilesRecursive(FileList, *Directory, TEXT("*.*"), true, false, false);
    return FileList;
}

TArray<FString> UUnrealAutoModBaseWidget::GetFileExtensionsFromPaths(const TArray<FString>& FilePaths)
{
    TArray<FString> Extensions;

    for (const FString& FilePath : FilePaths)
    {
        FString Extension = FPaths::GetExtension(FilePath);
        Extensions.Add(Extension);
    }

    return Extensions;
}

bool UUnrealAutoModBaseWidget::CreateDirectory(const FString& Directory)
{
    return IFileManager::Get().MakeDirectory(*Directory, true);
}

bool UUnrealAutoModBaseWidget::DeleteDirectoryAndContents(const FString& Directory)
{
    IFileManager& FileManager = IFileManager::Get();
    FileManager.DeleteDirectory(*Directory, false, true);
    return !FileManager.DirectoryExists(*Directory);
}
