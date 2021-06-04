#pragma once

#include "CoreMinimal.h"
#include "ItemSlotInfo.generated.h"

USTRUCT()
struct ARPG_API FItemSlotInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// 아이템 코드
	UPROPERTY()
	FName ItemCode;

	// 아이템 개수
	UPROPERTY()
	int32 ItemCount;

	// 슬롯에 저장될 수 있는 최대 개수
	UPROPERTY()
	int32 MaxSlotCount;

public :
	FItemSlotInfo();
	FItemSlotInfo(FName itemCode, int32 itemCount = 0, int32 maxSlotCount = 0);

	FORCEINLINE bool operator==(const FItemSlotInfo& itemSlotInfo)
	{
		return
			this->ItemCode == itemSlotInfo.ItemCode &&
			this->ItemCount == itemSlotInfo.ItemCount;
	}

	FORCEINLINE bool operator!=(const FItemSlotInfo& itemSlotInfo)
	{
		return
			this->ItemCode != itemSlotInfo.ItemCode ||
			this->ItemCount != itemSlotInfo.ItemCount;
	}

	// 같은 아이템인지 확인합니다.
	FORCEINLINE bool IsSameItem(const FItemSlotInfo& itemSlotInfo) const
	{
		if (itemSlotInfo.IsEmpty()) return false;

		return (ItemCode == itemSlotInfo.ItemCode);
	}


	// 해당 데이터가 비어있는지 확인합니다.
	FORCEINLINE bool IsEmpty() const
	{ return ItemCode.IsNone(); }

	// 해당 데이터를 비웁니다.
	FORCEINLINE void Clear()
	{ ItemCode = TEXT(""); }

};
