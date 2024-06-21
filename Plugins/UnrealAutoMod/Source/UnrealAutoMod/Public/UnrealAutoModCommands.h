// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "UnrealAutoModStyle.h"

class FUnrealAutoModCommands : public TCommands<FUnrealAutoModCommands>
{
public:

	FUnrealAutoModCommands()
		: TCommands<FUnrealAutoModCommands>(TEXT("UnrealAutoMod"), NSLOCTEXT("Contexts", "UnrealAutoMod", "UnrealAutoMod Plugin"), NAME_None, FUnrealAutoModStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};