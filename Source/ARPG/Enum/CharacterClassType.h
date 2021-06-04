#pragma once

#include "CoreMinimal.h"
#include "CharacterClassType.generated.h"

UENUM(BlueprintType)
enum class ECharacterClassType : uint8
{
	CLS_ALL		= 0 UMETA(DisplayName = ALL),
	CLS_Warrior	= 1 UMETA(DisplayName = 전사),
};
