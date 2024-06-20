// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
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