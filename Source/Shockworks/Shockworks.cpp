// Fill out your copyright notice in the Description page of Project Settings.

#include "Shockworks.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Shockworks, "Shockworks" );

/** Log categories.*/
DEFINE_LOG_CATEGORY(LogShockworks)
DEFINE_LOG_CATEGORY(LogShockworksSubsystem)
DEFINE_LOG_CATEGORY(LogShockworksEventActor)

/** Log categories for Reacoustic. Move to new module */
DEFINE_LOG_CATEGORY(LogReacousticSubsystem)
DEFINE_LOG_CATEGORY(LogReacousticComponent)



