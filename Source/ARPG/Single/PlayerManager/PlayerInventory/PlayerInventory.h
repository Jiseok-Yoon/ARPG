#pragma once

#include "ARPG.h"
#include "UObject/NoExportTypes.h"

#include "Struct/ItemSlotInfo/ItemSlotInfo.h"

#include "Enum/InputModeType.h"
#include "Enum/PartsType.h"


#include "PlayerInventory.generated.h"

#ifndef ITEM_ALL
#define ITEM_ALL -1
#endif

UCLASS()
class ARPG_API UPlayerInventory : public UObject
{
	GENERATED_BODY()

private :
	TSubclassOf<class UPlayerInventoryWnd> BP_PlayerInventoryWnd;
	TSubclassOf<class UPlayerEquipItemWnd> BP_EquipItemWnd;
	class UDataTable* DT_EquipItemInfo;

private :
	UPROPERTY()
	class UPlayerInventoryWnd* PlayerInventoryWnd;

	UPROPERTY()
	class UPlayerEquipItemWnd * PlayerEquipItemWnd;


public :
	UPlayerInventory();

public :
	// 인벤토리 창을 생성합니다.
	class UPlayerInventoryWnd* CreateInventoryWnd(
		class UWidgetControllerWidget* widgetController,
		EInputModeType changeInputMode = EInputModeType::IM_Default, bool bShowCursor = true);

	// 장비 아이템 창을 생성합니다.
	class UPlayerEquipItemWnd* CreateEquipItemWnd(
		class UWidgetControllerWidget* widgetController,
		EInputModeType changeInputMode = EInputModeType::IM_Default, bool bShowCursor = true);

	// 인벤토리 창을 닫습니다.
	void CloseInventoryWnd();

	// 장비 아이템 창을 닫습니다.
	void CloseEquipItemWnd();

	// 인벤토리 창을 토글합니다.
	void ToggleInventoryWnd(class UWidgetControllerWidget* widgetController, bool bOpenEquipItemWnd = false);

	// 장비 창을 토글합니다.
	void ToggleEquipItemWnd(class UWidgetControllerWidget* widgetController);

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
	///   ITEM_ALL 이 전달된다면 모든 아이템을 제거합니다.
	void RemoveItem(int32 itemSlotIndex, int32 removeCount = ITEM_ALL);

	// 장비 아이템을 장착합니다.
	/// - equipItemCode : 장착시키려는 아이템 코드를 전달합니다.
	/// - out_PrevItemSlotInfo : 이전에 장착시킨 아이템 정보를 받을 ItemSlotInfo 변수를 전달합니다.
	void EquipItem(FName equipItemCode, FItemSlotInfo* out_PrevItemSlotInfo = nullptr);

	// 장비 아이템을 장착 해제합니다.
	/// - partsType : 장착 해제시키려는 파츠를 전달합니다.
	/// - out_ItemSlotInfo : 이전에 장착시킨 아이템 정보를 받을 ItemSlotInfo 변수를 전달합니다.
	void DismountItem(EPartsType partsType, FItemSlotInfo* out_ItemSlotInfo = nullptr);

	// 플레이어 캐릭터의 비쥬얼을 갱신합니다.
	/// - 모든 Mesh 가 갱신됩니다.
	void UpdateCharacterVisual();

	// 플레이어 캐릭터 비쥬얼을 갱신합니다.
	/// - updateParts : 갱신시킬 파츠를 전달합니다.
	/// - equipItemInfo : 장비 아이템 정보를 전달합니다.
	///   - 만약 nullptr 를 전달한다면 이 메서드 내에서 캐릭터에 장착된 장비 정보를 이용하여 데이터를 얻게 됩니다.
	void UpdateCharacterVisual(EPartsType updateParts, struct FEquipItemInfo * equipItemInfo = nullptr);

public :
	FORCEINLINE class UPlayerInventoryWnd* GetPlayerInventoryWnd() const
	{ return PlayerInventoryWnd; }

	FORCEINLINE class UPlayerEquipItemWnd* GetPlayerEquipItemWnd() const
	{ return PlayerEquipItemWnd; }

};
