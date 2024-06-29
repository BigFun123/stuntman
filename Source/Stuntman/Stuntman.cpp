// Copyright Epic Games, Inc. All Rights Reserved.

#include "Stuntman.h"
#include "Modules/ModuleManager.h"
#include "Recorder/Recorder.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Stuntman, "Stuntman" );
 
// instantiate Recorder
Recorder r = Recorder::GetInstance();