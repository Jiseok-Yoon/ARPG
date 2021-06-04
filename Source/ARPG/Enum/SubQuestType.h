#pragma once

#include "CoreMinimal.h"
#include "SubQuestType.generated.h"

UENUM(BlueprintType)
enum class ESubQuestType : uint8
{
	CollectingSilver		UMETA(DisplayName = 은화 모으기),
	CollectingItem			UMETA(DisplayName = 아이템 모으기)
};
