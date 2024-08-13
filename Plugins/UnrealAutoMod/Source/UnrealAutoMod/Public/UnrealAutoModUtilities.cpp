#include "UnrealAutoModPrivatePCH.h"
#include "UnrealAutoModUtilities.h"
#include "PlatformProcess.h"
#include "Paths.h"
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
