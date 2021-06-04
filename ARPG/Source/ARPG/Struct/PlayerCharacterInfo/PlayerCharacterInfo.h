#pragma once

#include "CoreMinimal.h"

#include "Struct/ItemSlotInfo/ItemSlotInfo.h"

#include "PlayerCharacterInfo.generated.h"

USTRUCT()
struct ARPG_API FPlayerCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public :
	// 인벤토리 슬롯 개수
	UPROPERTY()
	int32 InventorySlotCount;

	// 소지중인 아이템 정보
	UPROPERTY()
	TArray<FItemSlotInfo> InventoryItemInfos;

	// 소지금
	UPROPERTY()
	int32 Silver;

public :
	FPlayerCharacterInfo();



};
