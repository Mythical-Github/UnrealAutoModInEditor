// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UnrealAutoMod.h"
#include "UnrealAutoModStyle.h"
#include "UnrealAutoModCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

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
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FUnrealAutoModModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("UnrealAutoMod.cpp"))
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