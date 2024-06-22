#include "UnrealAutoModPrivatePCH.h"
#include "UnrealAutoModBaseWidget.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h" // Include for launching processes
#include "Runtime/Core/Public/Misc/Paths.h" // Include for file path utilities
#include "UObject.h" // Include for UObject

void UUnrealAutoModBaseWidget::LaunchExternalExecutable(const FString& FilePath, const TArray<FString>& Parameters)
{
    // Construct the command string
    FString Command = FString::Printf(TEXT("\"%s\""), *FilePath);

    // Append each parameter to the command string
    for (const FString& Param : Parameters)
    {
        Command += FString::Printf(TEXT(" %s"), *Param);
    }

    // Launch the process
    FProcHandle ProcessHandle = FPlatformProcess::CreateProc(*FilePath, *Command, true, false, false, nullptr, 0, nullptr, nullptr);

    if (!ProcessHandle.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to launch executable: %s"), *FilePath);
    }
}

FString UUnrealAutoModBaseWidget::GetProjectDirectory()
{
    // Get the directory of the .uproject file
    FString ProjectFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());
    FString ProjectDir = FPaths::GetPath(ProjectFilePath); // Extract the directory part

    return ProjectDir;
}

UObject* UUnrealAutoModBaseWidget::LoadObjectByPath(const FString& ObjectPath)
{
    // Attempt to load the object from the given path
    return StaticLoadObject(UObject::StaticClass(), nullptr, *ObjectPath, nullptr, RF_NoFlags, nullptr);
}
