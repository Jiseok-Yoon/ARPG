#pragma once

#include "CoreMinimal.h"

#include "SlotType.generated.h"

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	ST_ShopItemSlot		UMETA(DisplayName = ShopItemSlot),
	ST_InventorySlot	UMETA(DisplayName = InventorySlot),
	ST_EquipSlot		UMETA(DisplayName = EquipSlot)
};