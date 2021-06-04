#pragma once

#include "EngineMinimal.h"

#ifndef USE_TAGS
#define USE_TAGS

#define PLAYER_ACTOR_TAG							TEXT("PlayerCharacter")
#endif

#ifndef FN_PTR_DEFINE
#define FN_PTR_DEFINE

#define Action(fnPtrName, ParamTypes)				void(*fnPtrName)ParamTypes
#define Func(RetType, fnPtrName, ParamTypes)		RetType(*fnPtrName)ParamTypes
#endif

#ifndef CUSTOM_LOG_DEFINE
#define CUSTOM_LOG_DEFINE
#define LOG(_log_, ...) UE_LOG(LogTemp, Warning, _log_, ##__VA_ARGS__)
#endif

