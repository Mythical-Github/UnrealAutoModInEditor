#include "UnrealAutoModPrivatePCH.h"
#include "UnrealAutoModBaseWidget.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h" // Include for launching processes
#include "Runtime/Core/Public/Misc/Paths.h" // Include for file path utilities

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

UObject* UUnrealAutoModBaseWidget::LoadObjectByPath(const FString& ObjectPath)
{
    return StaticLoadObject(UObject::StaticClass(), nullptr, *ObjectPath, nullptr, RF_NoFlags, nullptr);
}

bool UUnrealAutoModBaseWidget::IsValidObject(UObject* Object)
{
    return IsValid(Object);
}

bool UUnrealAutoModBaseWidget::IsValidClass(UClass* Class)
{
    return (Class != nullptr && Class->IsValidLowLevel());
}
