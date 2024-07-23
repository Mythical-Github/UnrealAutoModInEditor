
#pragma once

#include "Modules/ModuleManager.h"
#include "Blueprint/UserWidget.h"

class FToolBarBuilder;
class FMenuBuilder;

class FUnrealAutoModModule : public IModuleInterface
{
public:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    void PluginButtonClicked();

private:

    void AddToolbarExtension(FToolBarBuilder& Builder);
    void AddMenuExtension(FMenuBuilder& Builder);

    TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

    TSharedPtr<class FUICommandList> PluginCommands;
    UUserWidget* CachedUserWidget;
};
