#pragma once

#include "Widget/BaseSlot/ItemSlot/ItemSlot.h"
#include "PlayerInventoryItemSlot.generated.h"

UCLASS()
class ARPG_API UPlayerInventoryItemSlot : public UItemSlot
{
	GENERATED_BODY()
	
private :
	// 인벤토리 슬롯 인덱스를 나타냅니다.
	int32 ItemSlotIndex;


protected :
	virtual void NativeConstruct() override;

public :
	void InitializeItemSlot(ESlotType slotType, FName itemCode, int32 itemSlotIndex);

	// 아이템 슬롯을 갱신합니다.
	void UpdateInventoryItemSlot();

private :
	// 아이템 개수 텍스트를 갱신합니다.
	void UpdateItemCountText();

public :
	FORCEINLINE int32 GetItemSlotIndex() const
	{ return ItemSlotIndex; }



};
