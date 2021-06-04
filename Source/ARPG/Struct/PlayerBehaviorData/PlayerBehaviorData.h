#pragma once

#include "CoreMinimal.h"

#include "Enum/PlayerBehavior.h"

#include "PlayerBehaviorData.generated.h"

using namespace EPlayerBehavior;

USTRUCT()
struct ARPG_API FPlayerBehaviorData
{
	GENERATED_USTRUCT_BODY()

public :
	// 행동 타입을 나타냅니다.
	EPlayerBehavior::Type Behavior;

	// 아이템 코드, Npc 코드, 몬스터 코드 등을 나타냅니다.
	FName Code;

	// 수량을 나타냅니다.
	int32 Count;

	// 특정한 문자열이 전달됩니다.
	FString StringData1;

public:
	FPlayerBehaviorData(EPlayerBehavior::Type behavior = BH_None);
	FPlayerBehaviorData(EPlayerBehavior::Type behavior, FString stringData1);
	FPlayerBehaviorData(EPlayerBehavior::Type behavior, FName code, int32 count);

public :
	// 아이템 수를 얻습니다.
	FORCEINLINE int32 GetItemCount() const
	{ return Count; }
};
