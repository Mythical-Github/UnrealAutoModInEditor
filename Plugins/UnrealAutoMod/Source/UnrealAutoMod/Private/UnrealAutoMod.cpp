// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UnrealAutoMod.h" // Your module's header file
#include "UnrealAutoModPrivatePCH.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "UnrealAutoModStyle.h"
#include "UnrealAutoModCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Editor/EditorEngine.h"
#include "Editor.h"

static const FName UnrealAutoModTabName("UnrealAutoMod");

#define LOCTEXT_NAMESPACE "FUnrealAutoModModule"

void FUnrealAutoModModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

    FUnrealAutoModStyle::Initialize();
    FUnrealAutoModStyle::ReloadTextures();

    FUnrealAutoModCommands::Register();

    PluginCommands = MakeShareable(new FUICommandList);

    PluginCommands->MapAction(
        FUnrealAutoModCommands::Get().OpenPluginWindow,
        FExecuteAction::CreateRaw(this, &FUnrealAutoModModule::PluginButtonClicked),
        FCanExecuteAction());

    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

    {
        TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
        MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FUnrealAutoModModule::AddMenuExtension));

        LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
    }

    {
        TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
        ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FUnrealAutoModModule::AddToolbarExtension));

        LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
    }

    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(UnrealAutoModTabName, FOnSpawnTab::CreateRaw(this, &FUnrealAutoModModule::OnSpawnPluginTab))
        .SetDisplayName(LOCTEXT("FUnrealAutoModTabTitle", "UnrealAutoMod"))
        .SetMenuType(ETabSpawnerMenuType::Hidden);

    CachedUserWidget = nullptr; // Initialize the cached widget pointer
}

void FUnrealAutoModModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    FUnrealAutoModStyle::Shutdown();

    FUnrealAutoModCommands::Unregister();

    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(UnrealAutoModTabName);
}

TSharedRef<SDockTab> FUnrealAutoModModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
    if (!CachedUserWidget)
    {
        UClass* UserWidgetClass = LoadObject<UClass>(nullptr, TEXT("/UnrealAutoMod/Widgets/WBP_UnrealAutoMod.WBP_UnrealAutoMod_C"));

        if (!UserWidgetClass)
        {
            FText ErrorText = FText::Format(
                LOCTEXT("ErrorLoadingWidget", "Failed to load UserWidget class from {0}"),
                FText::FromString(TEXT("/UnrealAutoMod/Widgets/WBP_UnrealAutoMod.WBP_UnrealAutoMod_C"))
            );

            return SNew(SDockTab)
                .TabRole(ETabRole::NomadTab)
                [
                    SNew(SBox)
                    .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    SNew(STextBlock)
                    .Text(ErrorText)
                ]
                ];
        }

        UUserWidget* ExistingUserWidget = nullptr;
        for (TObjectIterator<UUserWidget> Itr; Itr; ++Itr)
        {
            if (Itr->IsA(UserWidgetClass))
            {
                ExistingUserWidget = *Itr;
                break;
            }
        }

        if (ExistingUserWidget)
        {
            CachedUserWidget = ExistingUserWidget;
        }
        else
        {
            CachedUserWidget = CreateWidget<UUserWidget>(GEditor->GetEditorWorldContext().World(), UserWidgetClass);
        }

        if (!CachedUserWidget)
        {
            FText ErrorText = FText::Format(
                LOCTEXT("ErrorCreatingWidget", "Failed to create UserWidget instance from {0}"),
                FText::FromString(TEXT("/UnrealAutoMod/Widgets/WBP_UnrealAutoMod.WBP_UnrealAutoMod_C"))
            );

            return SNew(SDockTab)
                .TabRole(ETabRole::NomadTab)
                [
                    SNew(SBox)
                    .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    SNew(STextBlock)
                    .Text(ErrorText)
                ]
                ];
        }
    }

    return SNew(SDockTab)
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SBox)
            .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            CachedUserWidget->TakeWidget()
        ]
        ];
}

void FUnrealAutoModModule::PluginButtonClicked()
{
    FGlobalTabmanager::Get()->InvokeTab(UnrealAutoModTabName);
}

void FUnrealAutoModModule::AddMenuExtension(FMenuBuilder& Builder)
{
    Builder.AddMenuEntry(FUnrealAutoModCommands::Get().OpenPluginWindow);
}

void FUnrealAutoModModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
    Builder.AddToolBarButton(FUnrealAutoModCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealAutoModModule, UnrealAutoMod)
