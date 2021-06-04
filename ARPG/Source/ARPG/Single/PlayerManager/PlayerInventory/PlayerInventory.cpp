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
	// 이미 인벤토리 창이 띄워져 있다면 띄워진 인벤토리 창을 반환합니다.
	if (IsValid(PlayerInventoryWnd)) return PlayerInventoryWnd;

	// 인벤토리 창을 생성합니다.
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
		// 아이템을 추가할 수 있는 여유 공간이 존재한다면
		int32 addableItemCount = inventoryItemInfos[slotIndex].MaxSlotCount - inventoryItemInfos[slotIndex].ItemCount;
		if (addableItemCount > 0)
		{
			// 추가할 수 있는 여유 공간을 매꾸며, 아이템을 최대한 채웁니다.
			for (int32 i = 0;
				((i < addableItemCount) && newItemSlotInfo.ItemCount > 0);
				++i)
			{
				// 아이템을 추가합니다.
				++inventoryItemInfos[slotIndex].ItemCount;

				// 추가한 아이템을 제외합니다.
				--newItemSlotInfo.ItemCount;
			}
		}
	};


	auto playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();
	TArray<FItemSlotInfo>& inventoryItemInfos = playerInfo->InventoryItemInfos;

	// 추가할 아이템 개수가 0 이라면 실행 X
	if (newItemSlotInfo.ItemCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInventory.cpp :: %d LINE :: ItemCount is zero!"), __LINE__);
		return true;
	}

	for (int32 i = 0; i < playerInfo->InventorySlotCount; ++i)
	{
		// 만약 추가하려는 아이템과 동일한 아이템을 갖는 슬롯을 찾았다면
		if (inventoryItemInfos[i].IsSameItem(newItemSlotInfo))
		{
			// 아이템 채우기
			fnFillSlot(newItemSlotInfo, inventoryItemInfos, i);

			// 인벤토리 창이 열려있다면 갱신합니다.
			if (IsValid(PlayerInventoryWnd))
				PlayerInventoryWnd->UpdateInventoryItemSlots();
		}
		// 빈 아이템 슬롯을 찾았다면
		else if (inventoryItemInfos[i].IsEmpty())
		{
			inventoryItemInfos[i] = newItemSlotInfo;
			inventoryItemInfos[i].ItemCount = 0;

			// 아이템 채우기
			fnFillSlot(newItemSlotInfo, inventoryItemInfos, i);

			// 인벤토리 창이 열려있다면 갱신합니다.
			if (IsValid(PlayerInventoryWnd))
				PlayerInventoryWnd->UpdateInventoryItemSlots();
		}

		// 모든 아이템을 추가했다면
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
			// 추가할 수 있는 여유 공간을 매꾸며, 아이템을 최대한 채웁니다.
			for (int32 i = 0; (i < addableItemCount) && oriItemInfo.ItemCount > 0; ++i)
			{
				// 아이템을 추가합니다.
				++targetItemInfo.ItemCount;

				// 추가한 아이템을 제외합니다.
				--oriItemInfo.ItemCount;
			}

			// 타깃 아이템에 원본 아이템이 다 합쳐졌다면
			if (oriItemInfo.ItemCount == 0)
			{
				oriItemInfo.Clear();
			}

			// 인벤토리 창이 열려있다면
			if (IsValid(PlayerInventoryWnd))
			{
				PlayerInventoryWnd->UpdateInventoryItemSlots();
			}
		}
		// 슬롯 카운트가 맥스인 아이템과 드래그 드랍했다면 스왑합니다.
		else if (addableItemCount == 0)
		{
			SwapItem(ori, target);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerInventory.cpp ::MergeItem() : targetItem.MaxSlotCount < targetItem.ItemCount !! %d LINE"), __LINE__)
		}
	}
	// 같은 아이템이 아니라면 스왑합니다.
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

	// 지정한 슬롯의 아이템을 removeCount 만큼 뺍니다.
	inventoryItemInfo[itemSlotIndex].ItemCount -= removeCount;

	// 아이템 개수가 0 이하라면 슬롯을 비웁니다.
	if (inventoryItemInfo[itemSlotIndex].ItemCount <= 0)
		inventoryItemInfo[itemSlotIndex].Clear();

	// 인벤토리 창이 열려있다면
	if (IsValid(playerInventoryWnd))
		// 인벤토리 창의 아이템 슬롯들을 갱신합니다.
		playerInventoryWnd->UpdateInventoryItemSlots();
}
