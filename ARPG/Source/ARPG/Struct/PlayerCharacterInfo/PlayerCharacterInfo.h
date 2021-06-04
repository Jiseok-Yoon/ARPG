#pragma once

#include "CoreMinimal.h"

#include "Struct/ItemSlotInfo/ItemSlotInfo.h"

#include "PlayerCharacterInfo.generated.h"

USTRUCT()
struct ARPG_API FPlayerCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public :
	// �κ��丮 ���� ����
	UPROPERTY()
	int32 InventorySlotCount;

	// �������� ������ ����
	UPROPERTY()
	TArray<FItemSlotInfo> InventoryItemInfos;

	// ������
	UPROPERTY()
	int32 Silver;

public :
	FPlayerCharacterInfo();



};
