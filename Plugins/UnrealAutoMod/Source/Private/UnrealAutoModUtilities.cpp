#include "UnrealAutoModUtilities.h"
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

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"
#include <windows.h>
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif

void UUnrealAutoModUtilities::LaunchExternalExecutableNoHead(const FString& FilePath, const TArray<FString>& Parameters)
{
    FString Command = FString::Printf(TEXT("\"%s\""), *FilePath);

    for (const FString& Param : Parameters)
    {
        Command += FString::Printf(TEXT(" %s"), *Param);
    }

#if PLATFORM_WINDOWS
    // Set up STARTUPINFO to hide the window for cmd.exe
    STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcessInfo;

    ZeroMemory(&StartupInfo, sizeof(StartupInfo));
    ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));

    StartupInfo.cb = sizeof(StartupInfo);
    StartupInfo.dwFlags |= STARTF_USESHOWWINDOW;
    StartupInfo.wShowWindow = SW_HIDE; // Hide the cmd.exe window

    // Create the full command to run via cmd.exe
    FString FullCommand = FString::Printf(TEXT("cmd.exe /C START /B \"%s\" %s"), *FilePath, *Command);
    TCHAR* CommandLine = FullCommand.GetCharArray().GetData();

    // Create process without window (CREATE_NO_WINDOW)
    if (CreateProcess(nullptr, CommandLine, nullptr, nullptr, false, CREATE_NO_WINDOW, nullptr, nullptr, &StartupInfo, &ProcessInfo))
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully launched executable headlessly: %s"), *FilePath);
        CloseHandle(ProcessInfo.hProcess);
        CloseHandle(ProcessInfo.hThread);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to launch executable headlessly: %s"), *FilePath);
    }

#else
    // For non-Windows platforms, fallback to FPlatformProcess (doesn't guarantee no window)
    FProcHandle ProcessHandle = FPlatformProcess::CreateProc(*FilePath, *Command, true, false, false, nullptr, 0, nullptr, nullptr);

    if (!ProcessHandle.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to launch executable headlessly: %s"), *FilePath);
}
#endif
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

#include "UnrealAutoModUtilities.h"
#include <fstream>
#include <sstream>
#include "Misc/MessageDialog.h"

FString UUnrealAutoModUtilities::ReadFile(const FString& FileName)
{
	std::ifstream FileStream(TCHAR_TO_UTF8(*FileName), std::ios::in | std::ios::binary);

	if (!FileStream.is_open())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to open file: %s"), *FileName);
		return FString();
	}

	std::stringstream Buffer;
	Buffer << FileStream.rdbuf();

	FileStream.close();

	// Convert the read content to FString
	std::string Content = Buffer.str();
	return FString(Content.c_str());
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
#if ENGINE_MAJOR_VERSION >= 5  // For Unreal Engine 5 and above
            EnumValues.Add(Enum->GetDisplayNameTextByIndex(i).ToString());
#else  // For Unreal Engine 4.x and below
            EnumValues.Add(Enum->GetNameByIndex(i).ToString());
#endif
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

bool UUnrealAutoModUtilities::WriteStringToFile(const FString& FileName, const FString& FileContents)
{
    // Use FFileHelper::SaveStringToFile to write the string to the file
    if (FFileHelper::SaveStringToFile(FileContents, *FileName))
    {
        return true; // Successfully written
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to write file: %s"), *FileName);
        return false; // Failed to write
    }
}

bool UUnrealAutoModUtilities::SetJsonFieldAsString(const FString& JsonString, const TArray<FString>& FieldNames, const FString& FieldValue, FString& OutJsonString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        TSharedPtr<FJsonObject> CurrentObject = JsonObject;

        for (int32 i = 0; i < FieldNames.Num() - 1; ++i)
        {
            if (CurrentObject->HasField(FieldNames[i]) && CurrentObject->GetField<EJson::Object>(FieldNames[i]).IsValid())
            {
                CurrentObject = CurrentObject->GetObjectField(FieldNames[i]);
            }
            else
            {
                TSharedPtr<FJsonObject> NewObject = MakeShareable(new FJsonObject());
                CurrentObject->SetObjectField(FieldNames[i], NewObject);
                CurrentObject = NewObject;
            }
        }

        // Determine if the FieldValue is a JSON string or a plain string
        TSharedPtr<FJsonObject> FieldValueObject;
        TSharedRef<TJsonReader<>> FieldValueReader = TJsonReaderFactory<>::Create(FieldValue);

        if (FJsonSerializer::Deserialize(FieldValueReader, FieldValueObject) && FieldValueObject.IsValid())
        {
            // FieldValue is a JSON object
            CurrentObject->SetObjectField(FieldNames.Last(), FieldValueObject);
        }
        else
        {
            // FieldValue is a plain string
            CurrentObject->SetStringField(FieldNames.Last(), FieldValue);
        }

        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJsonString);
        if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
        {
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to serialize updated JSON object."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON string."));
    }

    return false;
}

bool UUnrealAutoModUtilities::GetJsonFieldAsString(const FString& JsonString, const TArray<FString>& FieldNames, FString& FieldValue)
{
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    TSharedPtr<FJsonObject> JsonObject;

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        TSharedPtr<FJsonObject> CurrentObject = JsonObject;

        for (int32 i = 0; i < FieldNames.Num() - 1; ++i)
        {
            if (CurrentObject->HasField(FieldNames[i]) && CurrentObject->GetField<EJson::Object>(FieldNames[i]).IsValid())
            {
                CurrentObject = CurrentObject->GetObjectField(FieldNames[i]);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Field '%s' does not exist or is not a JSON object."), *FieldNames[i]);
                return false;
            }
        }

        if (CurrentObject->HasField(FieldNames.Last()))
        {
            TSharedPtr<FJsonValue> JsonValue = CurrentObject->TryGetField(FieldNames.Last());
            if (JsonValue.IsValid())
            {
                switch (JsonValue->Type)
                {
                case EJson::String:
                    FieldValue = JsonValue->AsString();
                    return true;

                case EJson::Number:
                    FieldValue = FString::Printf(TEXT("%f"), JsonValue->AsNumber());
                    return true;

                case EJson::Boolean:
                    FieldValue = JsonValue->AsBool() ? TEXT("true") : TEXT("false");
                    return true;

                case EJson::Object:
                {
                    TSharedPtr<FJsonObject> NestedObject = JsonValue->AsObject();
                    if (NestedObject.IsValid())
                    {
                        // Convert the nested object to a string representation
                        FString NestedObjectString;
                        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&NestedObjectString);
                        if (FJsonSerializer::Serialize(NestedObject.ToSharedRef(), Writer))
                        {
                            FieldValue = NestedObjectString;
                            return true;
                        }
                    }
                    break;
                }

                case EJson::Array:
                    UE_LOG(LogTemp, Error, TEXT("Json Value of type 'Array' cannot be used as 'String'."));
                    return false;

                case EJson::Null:
                    UE_LOG(LogTemp, Error, TEXT("Json Value is 'Null'."));
                    return false;

                default:
                    UE_LOG(LogTemp, Error, TEXT("Unknown Json Value type."));
                    return false;
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Field '%s' does not exist or is not valid."), *FieldNames.Last());
                return false;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Field '%s' does not exist."), *FieldNames.Last());
            return false;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON."));
        return false;
    }

    return false;
}

bool UUnrealAutoModUtilities::RemoveJsonField(const FString& JsonString, const TArray<FString>& FieldNames, FString& OutJsonString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        TSharedPtr<FJsonObject> CurrentObject = JsonObject;

        for (int32 i = 0; i < FieldNames.Num() - 1; ++i)
        {
            if (CurrentObject->HasField(FieldNames[i]) && CurrentObject->GetField<EJson::Object>(FieldNames[i]).IsValid())
            {
                CurrentObject = CurrentObject->GetObjectField(FieldNames[i]);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Field '%s' does not exist or is not a JSON object."), *FieldNames[i]);
                return false;
            }
        }

        if (CurrentObject->HasField(FieldNames.Last()))
        {
            CurrentObject->RemoveField(FieldNames.Last());

            TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJsonString);
            if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
            {
                return true;
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to serialize updated JSON object."));
                return false;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Field '%s' does not exist."), *FieldNames.Last());
            return false;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON string."));
        return false;
    }
}

bool UUnrealAutoModUtilities::ReadJsonField(const FString& JsonString, const TArray<FString>& FieldNames, FString& FieldValue)
{
    return GetJsonFieldAsString(JsonString, FieldNames, FieldValue);
}

bool UUnrealAutoModUtilities::AddJsonListEntry(const FString& JsonString, const TArray<FString>& FieldNames, const FString& EntryValue, FString& OutJsonString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        TSharedPtr<FJsonObject> CurrentObject = JsonObject;

        for (int32 i = 0; i < FieldNames.Num() - 1; ++i)
        {
            if (CurrentObject->HasField(FieldNames[i]) && CurrentObject->GetField<EJson::Object>(FieldNames[i]).IsValid())
            {
                CurrentObject = CurrentObject->GetObjectField(FieldNames[i]);
            }
            else
            {
                TSharedPtr<FJsonObject> NewObject = MakeShareable(new FJsonObject());
                CurrentObject->SetObjectField(FieldNames[i], NewObject);
                CurrentObject = NewObject;
            }
        }

        if (CurrentObject->HasField(FieldNames.Last()) && CurrentObject->GetField<EJson::Array>(FieldNames.Last()).IsValid())
        {
            TArray<TSharedPtr<FJsonValue>> Array = CurrentObject->GetArrayField(FieldNames.Last());

            TSharedPtr<FJsonValue> NewValue = MakeShareable(new FJsonValueString(EntryValue));
            Array.Add(NewValue);

            CurrentObject->SetArrayField(FieldNames.Last(), Array);

            TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJsonString);
            if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
            {
                return true;
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to serialize updated JSON object."));
                return false;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Field '%s' is not an array or does not exist."), *FieldNames.Last());
            return false;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON string."));
        return false;
    }
}

bool UUnrealAutoModUtilities::RemoveJsonListEntry(const FString& JsonString, const TArray<FString>& FieldNames, int32 Index, FString& OutJsonString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        TSharedPtr<FJsonObject> CurrentObject = JsonObject;

        for (int32 i = 0; i < FieldNames.Num() - 1; ++i)
        {
            if (CurrentObject->HasField(FieldNames[i]) && CurrentObject->GetField<EJson::Object>(FieldNames[i]).IsValid())
            {
                CurrentObject = CurrentObject->GetObjectField(FieldNames[i]);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Field '%s' does not exist or is not a JSON object."), *FieldNames[i]);
                return false;
            }
        }

        if (CurrentObject->HasField(FieldNames.Last()) && CurrentObject->GetField<EJson::Array>(FieldNames.Last()).IsValid())
        {
            TArray<TSharedPtr<FJsonValue>> Array = CurrentObject->GetArrayField(FieldNames.Last());

            if (Array.IsValidIndex(Index))
            {
                Array.RemoveAt(Index);
                CurrentObject->SetArrayField(FieldNames.Last(), Array);

                TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJsonString);
                if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
                {
                    return true;
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to serialize updated JSON object."));
                    return false;
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Index '%d' is out of bounds for array field '%s'."), Index, *FieldNames.Last());
                return false;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Field '%s' is not an array or does not exist."), *FieldNames.Last());
            return false;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON string."));
        return false;
    }
}

bool UUnrealAutoModUtilities::CheckIfJsonFieldExists(const FString& JsonString, const TArray<FString>& FieldNames, bool& Exists)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        TSharedPtr<FJsonObject> CurrentObject = JsonObject;

        for (int32 i = 0; i < FieldNames.Num(); ++i)
        {
            if (CurrentObject->HasField(FieldNames[i]))
            {
                if (i < FieldNames.Num() - 1)
                {
                    if (CurrentObject->GetField<EJson::Object>(FieldNames[i]).IsValid())
                    {
                        CurrentObject = CurrentObject->GetObjectField(FieldNames[i]);
                    }
                    else
                    {
                        Exists = false;
                        return true;
                    }
                }
            }
            else
            {
                Exists = false;
                return true;
            }
        }

        Exists = true;
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON string."));
        Exists = false;
        return false;
    }
}

bool UUnrealAutoModUtilities::GetJsonArrayEntries(const FString& JsonString, const TArray<FString>& FieldNames, TArray<FString>& Entries)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        TSharedPtr<FJsonObject> CurrentObject = JsonObject;

        for (int32 i = 0; i < FieldNames.Num(); ++i)
        {
            if (CurrentObject->HasField(FieldNames[i]))
            {
                if (i < FieldNames.Num() - 1)
                {
                    if (CurrentObject->GetField<EJson::Object>(FieldNames[i]).IsValid())
                    {
                        CurrentObject = CurrentObject->GetObjectField(FieldNames[i]);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Field '%s' is not an object."), *FieldNames[i]);
                        return false;
                    }
                }
                else
                {
                    const TArray<TSharedPtr<FJsonValue>>* JsonArray;
                    if (CurrentObject->TryGetArrayField(FieldNames[i], JsonArray))
                    {
                        Entries.Empty();
                        for (const TSharedPtr<FJsonValue>& JsonValue : *JsonArray)
                        {
                            if (JsonValue->Type == EJson::String)
                            {
                                Entries.Add(JsonValue->AsString());
                            }
                            else if (JsonValue->Type == EJson::Object)
                            {
                                TSharedPtr<FJsonObject> JsonObjectValue = JsonValue->AsObject();
                                FString JsonStringValue;
                                TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStringValue);
                                if (FJsonSerializer::Serialize(JsonObjectValue.ToSharedRef(), Writer))
                                {
                                    Entries.Add(JsonStringValue);
                                }
                            }
                            else
                            {
                                UE_LOG(LogTemp, Error, TEXT("Json Value of type '%d' used as 'String'."), (int32)JsonValue->Type);
                                return false;
                            }
                        }
                        return true;
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Field '%s' is not an array."), *FieldNames[i]);
                        return false;
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Field '%s' does not exist."), *FieldNames[i]);
                return false;
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON string."));
        return false;
    }

    return false;
}

FString UUnrealAutoModUtilities::GetParentDirectory(const FString& FilePath)
{
	return FPaths::GetPath(FilePath);
}
