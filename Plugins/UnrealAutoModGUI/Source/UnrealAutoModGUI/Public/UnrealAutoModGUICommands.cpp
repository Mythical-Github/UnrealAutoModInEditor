// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UnrealAutoModGUIPrivatePCH.h"
#include "UnrealAutoModGUICommands.h"

#define LOCTEXT_NAMESPACE "FUnrealAutoModGUIModule"

void FUnrealAutoModGUICommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "UnrealAutoMod", "Bring up UnrealAutoModGUI window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
