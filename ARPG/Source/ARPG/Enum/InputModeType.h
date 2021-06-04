#pragma once

#include "CoreMinimal.h"
#include "InputModeType.generated.h"

UENUM(BlueprintType)
enum class EInputModeType : uint8
{
	IM_Default		UMETA(DisplayName = Default),
	IM_GameOnly		UMETA(DisplayName = GameOnly),
	IM_UIOnly		UMETA(DisplayName = UIOnly),
	IM_GameAndUI	UMETA(DisplayName = GameAndUI)
};
