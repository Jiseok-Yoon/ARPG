#pragma once

#include "ARPG.h"
#include "UObject/NoExportTypes.h"

#include "Struct/ItemSlotInfo/ItemSlotInfo.h"

#include "Enum/InputModeType.h"


#include "PlayerInventory.generated.h"

UCLASS()
class ARPG_API UPlayerInventory : public UObject
{
	GENERATED_BODY()

private :
	TSubclassOf<class UPlayerInventoryWnd> BP_PlayerInventoryWnd;

private :
	UPROPERTY()
	class UPlayerInventoryWnd* PlayerInventoryWnd;

public :
	UPlayerInventory();

public :
	// 인벤토리 창을 생성합니다.
	class UPlayerInventoryWnd* CreateInventoryWnd(
		class UWidgetControllerWidget* widgetController,
		EInputModeType changeInputMode = EInputModeType::IM_Default, bool bShowCursor = true);

	// 인벤토리 창을 닫습니다.
	void CloseInventoryWnd();

	// 인벤토리 창을 토글합니다.
	void ToggleInventoryWnd(class UWidgetControllerWidget* widgetController);

	// 아이템 슬롯 정보를 얻습니다.
	FItemSlotInfo* GetItemSlotInfo(int32 slotIndex) const;

	// 인벤토리 아이템을 교체합니다.
	void SwapItem(class UPlayerInventoryItemSlot* first, class UPlayerInventoryItemSlot* second);
	
	// 아이템을 인벤토리에 추가합니다.
	bool AddItem(FItemSlotInfo& newItemSlotInfo);

	// 동일한 아이템을 합칩니다.
	void MergeItem(class UPlayerInventoryItemSlot* ori, class UPlayerInventoryItemSlot* target);

	// 아이템을 인벤토리에서 제거합니다.
	/// - itemSlotIndex : 제거할 슬롯 인덱스를 전달합니다.
	/// - removeCount : 제거할 개수를 전달합니다.
	void RemoveItem(int32 itemSlotIndex, int32 removeCount = 1);

public :
	FORCEINLINE class UPlayerInventoryWnd* GetPlayerInventoryWnd() const
	{ return PlayerInventoryWnd; }

};
