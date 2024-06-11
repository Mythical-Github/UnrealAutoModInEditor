// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "UnrealAutoModGUIStyle.h"

class FUnrealAutoModGUICommands : public TCommands<FUnrealAutoModGUICommands>
{
public:

	FUnrealAutoModGUICommands()
		: TCommands<FUnrealAutoModGUICommands>(TEXT("UnrealAutoMod"), NSLOCTEXT("Contexts", "UnrealAutoModGUI", "UnrealAutoModGUI Plugin"), NAME_None, FUnrealAutoModGUIStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};