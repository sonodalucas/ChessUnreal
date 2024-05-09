// Copyright Epic Games, Inc. All Rights Reserved.

#include "BugChess.h"
#include "Core/CustomLogging.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, BugChess, "BugChess" );

DEFINE_LOG_CATEGORY(LogChessGameMode);
DEFINE_LOG_CATEGORY(LogChessPlayerController);
DEFINE_LOG_CATEGORY(LogBCFunctionLibrary);