#pragma once

#include "CoreUObject.h"
#include "UnrealAutoModEnums.generated.h"

UENUM(BlueprintType)
enum class EWindowManagementAction : uint8
{
    NONE                    UMETA(DisplayName = "None"),
    MIN                     UMETA(DisplayName = "Min"),
    MAX                     UMETA(DisplayName = "Max"),
    MOVE                    UMETA(DisplayName = "Move"),
    CLOSE                   UMETA(DisplayName = "Close")
};

UENUM(BlueprintType)
enum class EPackingType : uint8
{
    ENGINE                  UMETA(DisplayName = "Engine"),
    UNREAL_PAK              UMETA(DisplayName = "Unreal Pak"),
    REPAK                   UMETA(DisplayName = "Repak"),
    LOOSE                   UMETA(DisplayName = "Loose")
};

UENUM(BlueprintType)
enum class EGameLaunchType : uint8
{
    EXE                     UMETA(DisplayName = "Exe"),
    STEAM                   UMETA(DisplayName = "Steam"),
    EPIC                    UMETA(DisplayName = "Epic"),
    ITCH_IO                 UMETA(DisplayName = "Itch.io"),
    BATTLE_NET              UMETA(DisplayName = "Battle.net"),
    ORIGIN                  UMETA(DisplayName = "Origin"),
    UBISOFT                 UMETA(DisplayName = "Ubisoft"),
    OTHER                   UMETA(DisplayName = "Other")
};

UENUM(BlueprintType)
enum class EScriptStateType : uint8
{
    NONE                    UMETA(DisplayName = "None"),
    PRE_ALL                 UMETA(DisplayName = "Pre All"),
    POST_ALL                UMETA(DisplayName = "Post All"),
    CONSTANT                UMETA(DisplayName = "Constant"),
    PRE_INIT                UMETA(DisplayName = "Pre Init"),
    INIT                    UMETA(DisplayName = "Init"),
    POST_INIT               UMETA(DisplayName = "Post Init"),
    PRE_COOKING             UMETA(DisplayName = "Pre Cooking"),
    POST_COOKING            UMETA(DisplayName = "Post Cooking"),
    PRE_MODS_UNINSTALL      UMETA(DisplayName = "Pre Mods Uninstall"),
    POST_MODS_UNINSTALL     UMETA(DisplayName = "Post Mods Uninstall"),
    PRE_PAK_DIR_SETUP       UMETA(DisplayName = "Pre Pak Dir Setup"),
    POST_PAK_DIR_SETUP      UMETA(DisplayName = "Post Pak Dir Setup"),
    PRE_MODS_INSTALL        UMETA(DisplayName = "Pre Mods Install"),
    POST_MODS_INSTALL       UMETA(DisplayName = "Post Mods Install"),
    PRE_GAME_LAUNCH         UMETA(DisplayName = "Pre Game Launch"),
    POST_GAME_LAUNCH        UMETA(DisplayName = "Post Game Launch"),
    PRE_GAME_CLOSE          UMETA(DisplayName = "Pre Game Close"),
    POST_GAME_CLOSE         UMETA(DisplayName = "Post Game Close"),
    PRE_ENGINE_OPEN         UMETA(DisplayName = "Pre Engine Open"),
    POST_ENGINE_OPEN        UMETA(DisplayName = "Post Engine Open"),
    PRE_ENGINE_CLOSE        UMETA(DisplayName = "Pre Engine Close"),
    POST_ENGINE_CLOSE       UMETA(DisplayName = "Post Engine Close")
};

UENUM(BlueprintType)
enum class EExecutionMode : uint8
{
    SYNC                    UMETA(DisplayName = "Sync"),
    ASYNC                   UMETA(DisplayName = "Async")
};

UENUM(BlueprintType)
enum class ECompressionType : uint8
{
    NONE                    UMETA(DisplayName = "None"),
    ZLIB                    UMETA(DisplayName = "Zlib"),
    GZIP                    UMETA(DisplayName = "Gzip"),
    OODLE                   UMETA(DisplayName = "Oodle"),
    ZSTD                    UMETA(DisplayName = "Zstd"),
    LZ4                     UMETA(DisplayName = "Lz4"),
    LZMA                    UMETA(DisplayName = "Lzma")
};

UENUM(BlueprintType)
enum class EUnrealModTreeType : uint8
{
    CUSTOM_CONTENT          UMETA(DisplayName = "Custom Content"), // "Content/CustomContent/ModName"
    MODS                    UMETA(DisplayName = "Mods")            // "Content/Mods/ModName"
};

UENUM(BlueprintType)
enum class EUtilityType : uint8
{
    RUN_EXE                 UMETA(DisplayName = "Run Exe"),
    OPEN_WEBSITE            UMETA(DisplayName = "Open Website"),
    OPEN_DIRECTORY          UMETA(DisplayName = "Open Directory")
};
