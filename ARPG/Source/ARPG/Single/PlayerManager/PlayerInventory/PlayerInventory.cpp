#include "PlayerInventory.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widget/BaseSlot/ItemSlot/PlayerInventoryItemSlot/PlayerInventoryItemSlot.h"

#include "Widget/ClosableWnd/PlayerInventoryWnd/PlayerInventoryWnd.h"
#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"

UPlayerInventory::UPlayerInventory()
{
	static ConstructorHelpers::FClassFinder<UPlayerInventoryWnd> BP_PLAYER_INVENTORY_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/InventoryWnd/BP_PlayerInventoryWnd.BP_PlayerInventoryWnd_C'"));
	if (BP_PLAYER_INVENTORY_WND.Succeeded()) BP_PlayerInventoryWnd = BP_PLAYER_INVENTORY_WND.Class;

}

UPlayerInventoryWnd* UPlayerInventory::CreateInventoryWnd(UWidgetControllerWidget* widgetController, EInputModeType changeInputMode, bool bShowCursor)
{
	// �̹� �κ��丮 â�� ����� �ִٸ� ����� �κ��丮 â�� ��ȯ�մϴ�.
	if (IsValid(PlayerInventoryWnd)) return PlayerInventoryWnd;

	// �κ��丮 â�� �����մϴ�.
	PlayerInventoryWnd = Cast<UPlayerInventoryWnd>(
		widgetController->CreateWnd(BP_PlayerInventoryWnd, true, changeInputMode, bShowCursor));
	PlayerInventoryWnd->OnWndClosedEvent.AddLambda(
		[this](class UClosableWnd * closableWnd) 
		{ PlayerInventoryWnd = nullptr;});


	return PlayerInventoryWnd;
}

void UPlayerInventory::CloseInventoryWnd()
{
	if (IsValid(PlayerInventoryWnd))
	{
		PlayerInventoryWnd->CloseThisWnd();
		PlayerInventoryWnd = nullptr;
	}
}

void UPlayerInventory::ToggleInventoryWnd(UWidgetControllerWidget* widgetController)
{
	if (IsValid(PlayerInventoryWnd))
		CloseInventoryWnd();
	else CreateInventoryWnd(widgetController, EInputModeType::IM_GameAndUI, true);
}

FItemSlotInfo* UPlayerInventory::GetItemSlotInfo(int32 slotIndex) const
{
	return &GetManager(UPlayerManager)->GetPlayerInfo()->InventoryItemInfos[slotIndex];
}

void UPlayerInventory::SwapItem(UPlayerInventoryItemSlot* first, UPlayerInventoryItemSlot* second)
{
	auto playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();
	TArray<FItemSlotInfo>& inventoryItemInfo = playerInfo->InventoryItemInfos;

	auto tempItemInfo = inventoryItemInfo[first->GetItemSlotIndex()];

	inventoryItemInfo[first->GetItemSlotIndex()] = inventoryItemInfo[second->GetItemSlotIndex()];
	inventoryItemInfo[second->GetItemSlotIndex()] = tempItemInfo;

	first->SetItemInfo(inventoryItemInfo[first->GetItemSlotIndex()].ItemCode);
	second->SetItemInfo(inventoryItemInfo[second->GetItemSlotIndex()].ItemCode);

	first->UpdateInventoryItemSlot();
	second->UpdateInventoryItemSlot();
}

bool UPlayerInventory::AddItem(FItemSlotInfo& newItemSlotInfo)
{
	Action(fnFillSlot, (FItemSlotInfo&, TArray<FItemSlotInfo>&, int32)) =
		[](FItemSlotInfo& newItemSlotInfo, TArray<FItemSlotInfo>& inventoryItemInfos, int32 slotIndex)
	{
		// �������� �߰��� �� �ִ� ���� ������ �����Ѵٸ�
		int32 addableItemCount = inventoryItemInfos[slotIndex].MaxSlotCount - inventoryItemInfos[slotIndex].ItemCount;
		if (addableItemCount > 0)
		{
			// �߰��� �� �ִ� ���� ������ �Ųٸ�, �������� �ִ��� ä��ϴ�.
			for (int32 i = 0;
				((i < addableItemCount) && newItemSlotInfo.ItemCount > 0);
				++i)
			{
				// �������� �߰��մϴ�.
				++inventoryItemInfos[slotIndex].ItemCount;

				// �߰��� �������� �����մϴ�.
				--newItemSlotInfo.ItemCount;
			}
		}
	};


	auto playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();
	TArray<FItemSlotInfo>& inventoryItemInfos = playerInfo->InventoryItemInfos;

	// �߰��� ������ ������ 0 �̶�� ���� X
	if (newItemSlotInfo.ItemCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInventory.cpp :: %d LINE :: ItemCount is zero!"), __LINE__);
		return true;
	}

	for (int32 i = 0; i < playerInfo->InventorySlotCount; ++i)
	{
		// ���� �߰��Ϸ��� �����۰� ������ �������� ���� ������ ã�Ҵٸ�
		if (inventoryItemInfos[i].IsSameItem(newItemSlotInfo))
		{
			// ������ ä���
			fnFillSlot(newItemSlotInfo, inventoryItemInfos, i);

			// �κ��丮 â�� �����ִٸ� �����մϴ�.
			if (IsValid(PlayerInventoryWnd))
				PlayerInventoryWnd->UpdateInventoryItemSlots();
		}
		// �� ������ ������ ã�Ҵٸ�
		else if (inventoryItemInfos[i].IsEmpty())
		{
			inventoryItemInfos[i] = newItemSlotInfo;
			inventoryItemInfos[i].ItemCount = 0;

			// ������ ä���
			fnFillSlot(newItemSlotInfo, inventoryItemInfos, i);

			// �κ��丮 â�� �����ִٸ� �����մϴ�.
			if (IsValid(PlayerInventoryWnd))
				PlayerInventoryWnd->UpdateInventoryItemSlots();
		}

		// ��� �������� �߰��ߴٸ�
		if (newItemSlotInfo.ItemCount == 0)
			return true;
	}

	return false;
}

void UPlayerInventory::MergeItem(UPlayerInventoryItemSlot* ori, UPlayerInventoryItemSlot* target)
{
	auto playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();
	TArray<FItemSlotInfo>& inventoryItemInfos = playerInfo->InventoryItemInfos;

	FItemSlotInfo& oriItemInfo = inventoryItemInfos[ori->GetItemSlotIndex()];
	FItemSlotInfo& targetItemInfo = inventoryItemInfos[target->GetItemSlotIndex()];

	if (oriItemInfo.IsSameItem(targetItemInfo))
	{
		int32 addableItemCount = targetItemInfo.MaxSlotCount - targetItemInfo.ItemCount;

		if (addableItemCount > 0)
		{
			// �߰��� �� �ִ� ���� ������ �Ųٸ�, �������� �ִ��� ä��ϴ�.
			for (int32 i = 0; (i < addableItemCount) && oriItemInfo.ItemCount > 0; ++i)
			{
				// �������� �߰��մϴ�.
				++targetItemInfo.ItemCount;

				// �߰��� �������� �����մϴ�.
				--oriItemInfo.ItemCount;
			}

			// Ÿ�� �����ۿ� ���� �������� �� �������ٸ�
			if (oriItemInfo.ItemCount == 0)
			{
				oriItemInfo.Clear();
			}

			// �κ��丮 â�� �����ִٸ�
			if (IsValid(PlayerInventoryWnd))
			{
				PlayerInventoryWnd->UpdateInventoryItemSlots();
			}
		}
		// ���� ī��Ʈ�� �ƽ��� �����۰� �巡�� ����ߴٸ� �����մϴ�.
		else if (addableItemCount == 0)
		{
			SwapItem(ori, target);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerInventory.cpp ::MergeItem() : targetItem.MaxSlotCount < targetItem.ItemCount !! %d LINE"), __LINE__)
		}
	}
	// ���� �������� �ƴ϶�� �����մϴ�.
	else
	{
		SwapItem(ori, target);
	}

}

void UPlayerInventory::RemoveItem(int32 itemSlotIndex, int32 removeCount)
{
	auto playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();
	auto playerInventoryWnd = PlayerInventoryWnd;

	TArray<FItemSlotInfo>& inventoryItemInfo = playerInfo->InventoryItemInfos;

	// ������ ������ �������� removeCount ��ŭ ���ϴ�.
	inventoryItemInfo[itemSlotIndex].ItemCount -= removeCount;

	// ������ ������ 0 ���϶�� ������ ���ϴ�.
	if (inventoryItemInfo[itemSlotIndex].ItemCount <= 0)
		inventoryItemInfo[itemSlotIndex].Clear();

	// �κ��丮 â�� �����ִٸ�
	if (IsValid(playerInventoryWnd))
		// �κ��丮 â�� ������ ���Ե��� �����մϴ�.
		playerInventoryWnd->UpdateInventoryItemSlots();
}
