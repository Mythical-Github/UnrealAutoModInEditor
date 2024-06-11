// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UnrealAutoModGUIPrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "UnrealAutoModGUIStyle.h"
#include "UnrealAutoModGUICommands.h"

#include "LevelEditor.h"

static const FName UnrealAutoModGUITabName("UnrealAutoModGUI");

#define LOCTEXT_NAMESPACE "FUnrealAutoModGUIModule"

void FUnrealAutoModGUIModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FUnrealAutoModGUIStyle::Initialize();
	FUnrealAutoModGUIStyle::ReloadTextures();

	FUnrealAutoModGUICommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUnrealAutoModGUICommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FUnrealAutoModGUIModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FUnrealAutoModGUIModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FUnrealAutoModGUIModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(UnrealAutoModGUITabName, FOnSpawnTab::CreateRaw(this, &FUnrealAutoModGUIModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FUnrealAutoModGUITabTitle", "UnrealAutoModGUI"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FUnrealAutoModGUIModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FUnrealAutoModGUIStyle::Shutdown();

	FUnrealAutoModGUICommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(UnrealAutoModGUITabName);
}

TSharedRef<SDockTab> FUnrealAutoModGUIModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FUnrealAutoModGUIModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("UnrealAutoModGUI.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FUnrealAutoModGUIModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(UnrealAutoModGUITabName);
}

void FUnrealAutoModGUIModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FUnrealAutoModGUICommands::Get().OpenPluginWindow);
}

void FUnrealAutoModGUIModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FUnrealAutoModGUICommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealAutoModGUIModule, UnrealAutoModGUI)