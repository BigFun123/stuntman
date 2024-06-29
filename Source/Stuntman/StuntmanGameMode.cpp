// Copyright Epic Games, Inc. All Rights Reserved.

#include "StuntmanGameMode.h"
#include "StuntmanPlayerController.h"

AStuntmanGameMode::AStuntmanGameMode()
{
	PlayerControllerClass = AStuntmanPlayerController::StaticClass();
}
