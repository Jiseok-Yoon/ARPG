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
	// �κ��丮 â�� �����մϴ�.
	class UPlayerInventoryWnd* CreateInventoryWnd(
		class UWidgetControllerWidget* widgetController,
		EInputModeType changeInputMode = EInputModeType::IM_Default, bool bShowCursor = true);

	// �κ��丮 â�� �ݽ��ϴ�.
	void CloseInventoryWnd();

	// �κ��丮 â�� ����մϴ�.
	void ToggleInventoryWnd(class UWidgetControllerWidget* widgetController);

	// ������ ���� ������ ����ϴ�.
	FItemSlotInfo* GetItemSlotInfo(int32 slotIndex) const;

	// �κ��丮 �������� ��ü�մϴ�.
	void SwapItem(class UPlayerInventoryItemSlot* first, class UPlayerInventoryItemSlot* second);
	
	// �������� �κ��丮�� �߰��մϴ�.
	bool AddItem(FItemSlotInfo& newItemSlotInfo);

	// ������ �������� ��Ĩ�ϴ�.
	void MergeItem(class UPlayerInventoryItemSlot* ori, class UPlayerInventoryItemSlot* target);

	// �������� �κ��丮���� �����մϴ�.
	/// - itemSlotIndex : ������ ���� �ε����� �����մϴ�.
	/// - removeCount : ������ ������ �����մϴ�.
	void RemoveItem(int32 itemSlotIndex, int32 removeCount = 1);

public :
	FORCEINLINE class UPlayerInventoryWnd* GetPlayerInventoryWnd() const
	{ return PlayerInventoryWnd; }

};
