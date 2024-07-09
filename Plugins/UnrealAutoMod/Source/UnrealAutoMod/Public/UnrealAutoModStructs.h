
#pragma once 

#include "CoreUObject.h"
#include "Engine/DataTable.h"
#include "UnrealAutoModEnums.h"
#include "UnrealAutoModBaseWidget.h"
#include "Materials/MaterialInterface.h" 
#include "UnrealAutoModStructs.generated.h"

USTRUCT(BlueprintType)
struct FProgramRunnerEntry : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString ProgramPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    EScriptStateType ScriptState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    EExecutionMode ExecutionMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TArray<FString> LaunchArgs;
};

USTRUCT(BlueprintType)
struct FWindowManagementEntry : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString WindowName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bUseSubstringCheck;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    EWindowManagementAction WindowState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    EScriptStateType ScriptState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    int MonitorIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    int ResolutionX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    int ResolutionY;
};

USTRUCT(BlueprintType)
struct FProcessManagementEntry : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString ProcessName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bUseSubstringCheck;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    EScriptStateType ScriptState;
};

USTRUCT(BlueprintType)
struct FProcessManagementList : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bAutoCloseGame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TArray<FProcessManagementEntry> ProcessManagementEntries;
};

USTRUCT(BlueprintType)
struct FAltUprojectNameInGameDirectory : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bUseAltMethod;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString Name;
};

USTRUCT(BlueprintType)
struct FGameInfo : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString GameExePath; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    EGameLaunchType LaunchType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bOverrideAutomaticLauncherExeFinding;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString GameLauncherExe;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString GameID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bSkipLaunchingGame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bOverrideAutomaticWindowTitleFinding;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString bWindowTitleOverrideString;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TArray<FString> bLaunchParameters;
};

USTRUCT(BlueprintType)
struct FGeneralInfo : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bOverrideDefaultWorkingDirectory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString WorkingDirectory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString WindowTitle;
};

USTRUCT(BlueprintType)
struct FRepakInfo : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString RepakExePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bOverrideAutomaticVersionFinding;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString RepakVersion;
};

USTRUCT(BlueprintType)
struct FMainMenuWidgetEntry : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TSubclassOf<UUnrealAutoModBaseWidget> MainMenuWidget;
};

USTRUCT(BlueprintType)
struct FModEntry : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString ModName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bIsEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString PakDirectoryStructure;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    EUnrealModTreeType ModNameDirectoryType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool UseModNameDirectoryNameOverride;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString ModNameDirectoryOverride;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    int32 PakChunkNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    EPackingType PackingType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    ECompressionType CompressionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TArray<FString> AssetPaths;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TArray<FString> TreePaths;
};

USTRUCT(BlueprintType)
struct FButtonInfo : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FText ButtonText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TSubclassOf<UObject> Object;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FString FunctionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TArray<FString> FunctionParameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    UMaterialInterface* ButtonMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    float TransformAngle;
};

USTRUCT(BlueprintType)
struct FEngineInfo : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bToggleEngineDuringTesting;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bResavePackagesAndFixUpRedirectorsBeforeEngineOpen;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TArray<FString> EngineLaunchArgs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TArray<FString> EngineCookAndPackagingArgs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bUseUnversionedCookedContent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bClearUprojectSavedDirectoryBeforeTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    bool bAlwaysBuildProject;

};

USTRUCT(BlueprintType)
struct FUnrealAutoModJson : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FGeneralInfo GeneralInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FEngineInfo EngineInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FGameInfo GameInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FAltUprojectNameInGameDirectory AltUprojectNameInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FRepakInfo RepakInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TArray<FProgramRunnerEntry> ProgramRunnerInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FProcessManagementList FProcessManagementInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TArray<FWindowManagementEntry> WindowManagementInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TArray<FModEntry> ModsInfo;
};

USTRUCT(BlueprintType)
struct FUnrealAutoModTheme : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FLinearColor BackgroundTopColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FLinearColor BackgroundMiddleColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FLinearColor BackgroundDividerColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FLinearColor BackgroundBottomColor;
};

USTRUCT(BlueprintType)
struct FMenuTemplate : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    FText TitleText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealAutoMod")
    TSubclassOf<UUnrealAutoModBaseWidget> WidgetClass;
};
