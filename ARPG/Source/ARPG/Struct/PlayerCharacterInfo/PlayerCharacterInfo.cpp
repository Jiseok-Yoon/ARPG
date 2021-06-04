#include "PlayerCharacterInfo.h"

FPlayerCharacterInfo::FPlayerCharacterInfo()
{
	InventorySlotCount = 50;

	for (int32 i = 0; i < InventorySlotCount; ++i)
		InventoryItemInfos.Add(FItemSlotInfo());

	// TEST
	Silver = 10000;
	InventoryItemInfos[1] = (FItemSlotInfo(FName(TEXT("90018")), 3, 10));
	InventoryItemInfos[12] = (FItemSlotInfo(FName(TEXT("90017")), 10, 10));


}
