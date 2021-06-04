#include "PlayerBehaviorData.h"

FPlayerBehaviorData::FPlayerBehaviorData(EPlayerBehavior::Type behavior) : 
	FPlayerBehaviorData(behavior, FName(), 0) { }

FPlayerBehaviorData::FPlayerBehaviorData(EPlayerBehavior::Type behavior, FString stringData1) : 
	FPlayerBehaviorData(behavior, FName(), 0)
{
	StringData1 = stringData1;
}

FPlayerBehaviorData::FPlayerBehaviorData(EPlayerBehavior::Type behavior, FName code, int32 count)
{
	Behavior = behavior;
	Code = code;
	Count = count;
	StringData1 = FString();
}
