// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UnrealAutoModCommands.h"

#define LOCTEXT_NAMESPACE "FUnrealAutoModModule"

void FUnrealAutoModCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "UnrealAutoMod", "Bring up UnrealAutoMod window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
