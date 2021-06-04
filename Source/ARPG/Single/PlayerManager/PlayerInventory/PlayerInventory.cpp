#include "PlayerInventory.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"
#include "Struct/EquipItemInfo/EquipItemInfo.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Actor/Controller/PlayerController/BasePlayerController.h"

#include "Widget/BaseSlot/ItemSlot/PlayerInventoryItemSlot/PlayerInventoryItemSlot.h"

#include "Widget/ClosableWnd/PlayerInventoryWnd/PlayerInventoryWnd.h"
#include "Widget/ClosableWnd/PlayerEquipItemWnd/PlayerEquipItemWnd.h"
#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"

#include "Component/PlayerBehaviorBroadcast/PlayerBehaviorBroadcastComponent.h"

#include "Components/CanvasPanelSlot.h"


UPlayerInventory::UPlayerInventory()
{
	static ConstructorHelpers::FClassFinder<UPlayerInventoryWnd> BP_PLAYER_INVENTORY_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/InventoryWnd/BP_PlayerInventoryWnd.BP_PlayerInventoryWnd_C'"));
	if (BP_PLAYER_INVENTORY_WND.Succeeded()) BP_PlayerInventoryWnd = BP_PLAYER_INVENTORY_WND.Class;

	static ConstructorHelpers::FClassFinder<UPlayerEquipItemWnd> BP_PLAYER_EQUIP_ITEM_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/EquipItemWnd/BP_EquipItemWnd.BP_EquipItemWnd_C'"));
	if (BP_PLAYER_EQUIP_ITEM_WND.Succeeded()) BP_EquipItemWnd = BP_PLAYER_EQUIP_ITEM_WND.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_EQUIP_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EquipItemInfo.DT_EquipItemInfo'"));
	if (DT_EQUIP_ITEM_INFO.Succeeded()) DT_EquipItemInfo = DT_EQUIP_ITEM_INFO.Object;
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

UPlayerEquipItemWnd* UPlayerInventory::CreateEquipItemWnd(
	UWidgetControllerWidget* widgetController, EInputModeType changeInputMode, bool bShowCursor)
{
	if (IsValid(PlayerEquipItemWnd)) return PlayerEquipItemWnd;

	// 창을 띄웁니다.
	PlayerEquipItemWnd = Cast<UPlayerEquipItemWnd>(
		widgetController->CreateWnd(BP_EquipItemWnd, true, changeInputMode, bShowCursor));

	// 창 닫힘 이벤트 추가
	PlayerEquipItemWnd->OnWndClosedEvent.AddLambda([this](class UClosableWnd* closableWnd)
		{ PlayerEquipItemWnd = nullptr; });

	// 인벤토리 창이 열려있다면
	if (IsValid(PlayerInventoryWnd))
	{
		// 인벤토리 절반 크기를 저장합니다.
		FVector2D inventoryHalfSize = PlayerInventoryWnd->GetWndSize() * 0.5f;

		// 인벤토리 위치를 저장합니다.
		FVector2D inventoryWndPos = PlayerInventoryWnd->GetCanvasPanelSlot()->GetPosition();

		// 장비창 절반 크기를 저장합니다.
		FVector2D equipItemWndHalfSize = PlayerEquipItemWnd->GetWndSize() * 0.5f;

		// 장비창의 위치를 계산합니다.
		FVector2D equipItemWndPos =
			inventoryWndPos + (inventoryHalfSize + equipItemWndHalfSize) * FVector2D(-1.0f, 0.0f);

		equipItemWndPos.Y += equipItemWndHalfSize.Y - inventoryHalfSize.Y;
		//equipItemWndPos.Y += inventoryHalfSize.Y - equipItemWndHalfSize.Y;

		// 장비 창의 위치를 설정합니다.
		PlayerEquipItemWnd->GetCanvasPanelSlot()->SetPosition(equipItemWndPos);
	}



	return PlayerEquipItemWnd;
}

void UPlayerInventory::CloseInventoryWnd()
{
	if (IsValid(PlayerInventoryWnd))
	{
		PlayerInventoryWnd->CloseThisWnd();
		PlayerInventoryWnd = nullptr;
	}
}

void UPlayerInventory::CloseEquipItemWnd()
{
	if (IsValid(PlayerEquipItemWnd))
	{
		PlayerEquipItemWnd->CloseThisWnd();
		PlayerEquipItemWnd = nullptr;
	}
}

void UPlayerInventory::ToggleInventoryWnd(UWidgetControllerWidget* widgetController, bool bOpenEquipItemWnd)
{
	if (IsValid(PlayerInventoryWnd))
	{
		CloseInventoryWnd();
		CloseEquipItemWnd();
	}
	else
	{
		CreateInventoryWnd(widgetController, EInputModeType::IM_GameAndUI, true);

		if (bOpenEquipItemWnd)
			CreateEquipItemWnd(widgetController, EInputModeType::IM_GameAndUI, true);

	}
}

void UPlayerInventory::ToggleEquipItemWnd(UWidgetControllerWidget* widgetController)
{
	if (IsValid(PlayerEquipItemWnd))
		CloseEquipItemWnd();
	else CreateEquipItemWnd(widgetController, EInputModeType::IM_GameAndUI, true);
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

	// 행동 이벤트 관리 객체
	auto& playerBehaviorBroadcast = *GetManager(UPlayerManager)->GetPlayerController()->GetPlayerBehaviorBroadcast();

	// 추가할 아이템 개수를 저장합니다.
	/// - 행동 이벤트를 위해 사용됩니다.
	int32 prevItemCount = newItemSlotInfo.ItemCount;

	// 추가할 아이템 개수가 0 이라면 실행 X
	if (newItemSlotInfo.ItemCount <= 0)
	{
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
		{
			// BH_GetItem [인벤토리에 모든 아이템 추가 시]
			FPlayerBehaviorData bhData(BH_GetItem, newItemSlotInfo.ItemCode, prevItemCount);
			playerBehaviorBroadcast.AddBehavior(bhData);
			return true;
		}
	}

	// 아이템을 하나라도 추가했다면
	if (prevItemCount != newItemSlotInfo.ItemCount)
	{
		// 추가된 아이템 개수를 얻습니다.
		int32 addedItemCount = prevItemCount - newItemSlotInfo.ItemCount;

		// BH_GetItem [인벤토리에 아이템 추가 시]
		FPlayerBehaviorData bhData(BH_GetItem, newItemSlotInfo.ItemCode, addedItemCount);
		playerBehaviorBroadcast.AddBehavior(bhData);
	}


	return false;
}

void UPlayerInventory::MergeItem(UPlayerInventoryItemSlot* ori, UPlayerInventoryItemSlot* target)
{
	auto playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();

	FItemSlotInfo& oriItemSlotInfo = playerInfo->InventoryItemInfos[ori->GetItemSlotIndex()];
	FItemSlotInfo& targetItemSlotInfo = playerInfo->InventoryItemInfos[target->GetItemSlotIndex()];

	// 슬롯에 들어갈 수 있는 최대 아이템 개수
	int32 maxSlotCount = ori->GetItemInfo()->MaxSlotCount;

	// 둘 중 하나라도 최대 개수라면 스왑이 일어나도록 합니다.
	if (oriItemSlotInfo.ItemCount == maxSlotCount ||
		targetItemSlotInfo.ItemCount == maxSlotCount)
		SwapItem(ori, target);
	else
	{
		// 추가 가능한 아이템 개수를 계산합니다.
		int32 addable = maxSlotCount - targetItemSlotInfo.ItemCount;

		// 옮기려는 아이템의 개수가 슬롯의 아이템 개수보다 큰 경우
		if (addable > oriItemSlotInfo.ItemCount)
			// 옮길 개수를 슬롯 아아템 개수로 설정합니다.
			addable = oriItemSlotInfo.ItemCount;

		// 아이템을 옮깁니다.
		oriItemSlotInfo.ItemCount -= addable;
		targetItemSlotInfo.ItemCount += addable;

		// 옮긴 후 슬롯이 비어있다면
		if (oriItemSlotInfo.ItemCount == 0)
		{
			oriItemSlotInfo.Clear();
			ori->SetItemInfo(FName());
		}

		// 슬롯 갱신
		ori->UpdateInventoryItemSlot();
		target->UpdateInventoryItemSlot();
	}
}

void UPlayerInventory::RemoveItem(int32 itemSlotIndex, int32 removeCount)
{
	auto playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();
	auto playerInventoryWnd = PlayerInventoryWnd;

	TArray<FItemSlotInfo>& inventoryItemInfo = playerInfo->InventoryItemInfos;

	// 지정한 슬롯의 아이템을 removeCount 만큼 뺍니다.
	/// - removeCount 에 ITEM_ALL 이 전달되었다면 완전히 비웁니다.
	inventoryItemInfo[itemSlotIndex].ItemCount -= 
		(removeCount == ITEM_ALL) ? 
		inventoryItemInfo[itemSlotIndex].ItemCount : 
		removeCount;

	// 아이템 개수가 0 이하라면 슬롯을 비웁니다.
	if (inventoryItemInfo[itemSlotIndex].ItemCount <= 0)
		inventoryItemInfo[itemSlotIndex].Clear();

	// 인벤토리 창이 열려있다면
	if (IsValid(playerInventoryWnd))
		// 인벤토리 창의 아이템 슬롯들을 갱신합니다.
		playerInventoryWnd->UpdateInventoryItemSlots();
}

void UPlayerInventory::EquipItem(FName equipItemCode, FItemSlotInfo* out_PrevItemSlotInfo)
{
	// 장비 아이템 정보를 얻습니다.
	FString contextString;
	FEquipItemInfo* equipItemInfo = DT_EquipItemInfo->FindRow<FEquipItemInfo>(equipItemCode, contextString);

	if (equipItemInfo == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInventory.cpp :: %d LINE :: equipItemCode(%s) is not valid!"),
			__LINE__, *equipItemCode.ToString());
		return;
	}

	// 이전 아이템 정보를 전달받을 변수가 전달되었다면
	if (out_PrevItemSlotInfo)
	{
		// 이전에 장착된 아이템 정보를 얻습니다.
		bool bReturnedAvailableData;
		auto prevEquipItemData = GetManager(UPlayerManager)->GetPlayerInfo()->GetEquippedItemSlotInfo(
			equipItemInfo->PartsType, &bReturnedAvailableData);

		// 이전에 장착된 장비가 존재하지 않는다면
		/// - 기본 장비라면 이전에 장착된 장비가 존재하지 않음
		if (prevEquipItemData.Get<1>())
			// 빈 정보를 반환시킵니다.
			*out_PrevItemSlotInfo = FItemSlotInfo();

		// 이전에 장착된 장비가 존재한다면, 장비 정보를 반환합니다.
		else *out_PrevItemSlotInfo = prevEquipItemData.Get<0>();
	}

	// 장비 아이템 장착
	GetManager(UPlayerManager)->GetPlayerInfo()->PartsInfos[equipItemInfo->PartsType] =
		FItemSlotInfo(equipItemCode, 1, 1);

	// 비쥬얼 갱신
	UpdateCharacterVisual(equipItemInfo->PartsType, equipItemInfo);
}

void UPlayerInventory::DismountItem(EPartsType partsType, FItemSlotInfo* out_ItemSlotInfo)
{
	// 이전에 장착된 아이템 정보를 얻습니다.
	auto prevEquipitemData = GetManager(UPlayerManager)->GetPlayerInfo()->GetEquippedItemSlotInfo(partsType);

	// 이전에 장착된 아이템 정보를 받을 변수가 전달되었다면
	if (out_ItemSlotInfo)
	{
		// 이전에 장착된 장비가 존재하지 않는다면
		if (prevEquipitemData.Get<1>())
			*out_ItemSlotInfo = FItemSlotInfo();

		// 이전에 장착된 장비가 존재한다면
		else *out_ItemSlotInfo = prevEquipitemData.Get<0>();
	}

	// 장비 아이템 해제
	GetManager(UPlayerManager)->GetPlayerInfo()->PartsInfos[partsType] = FItemSlotInfo();

	// 비쥬얼 갱신
	UpdateCharacterVisual(partsType);

}

void UPlayerInventory::UpdateCharacterVisual()
{
	// 장착 가능한 모든 파츠 타입을 얻습니다.
	TArray<EPartsType> equipItemPartsTypes;
	GetManager(UPlayerManager)->GetPlayerInfo()->PartsInfos.GetKeys(equipItemPartsTypes);
	/// - TMap.GetKeys(TArray<KeyType>* outKeys) : TMap 요소의 모든 키를 배열로 반환합니다.
	/// - outKeys : 출력용 매개 변수이며, TMap 요소들의 키를 저장할 배열을 전달합니다.
	
	// 비쥬얼 갱신
	for (auto partsType : equipItemPartsTypes)
		UpdateCharacterVisual(partsType);
}

void UPlayerInventory::UpdateCharacterVisual(EPartsType updateParts, FEquipItemInfo* equipItemInfo)
{
	// 플레이어 캐릭터 정보
	FPlayerCharacterInfo* playerCharacterInfo = GetManager(UPlayerManager)->GetPlayerInfo();

	// 현재 장착된 파츠중 updateParts 와 일치하는 파츠 정보를 얻습니다.
	bool bReturnedAvailableData = false; // 반환된 데이터가 유효한 데이터임을 확인하기 위한 변수
	TTuple<FItemSlotInfo, bool> equippedPartsData = playerCharacterInfo->GetEquippedItemSlotInfo(
		updateParts, &bReturnedAvailableData);

	// 유효한 데이터가 아니라면 리턴
	if (!bReturnedAvailableData) return;

	// 현재 장착된 장비 아이템 슬롯 정보
	FItemSlotInfo* equippedItemSlotInfo = &equippedPartsData.Get<0>();

	// 유효한 데이터이지만, 정보가 비어있다면 리턴
	if (equippedItemSlotInfo->IsEmpty()) return;

	// 플레이어 캐릭터 액터
	APlayerCharacter* playerCharacter = GetManager(UPlayerManager)->GetPlayerCharacter();

	// 플레이어 캐릭터가 유효하지 않다면 메서드 종료
	if (!IsValid(playerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInventory.cpp :: %d LINE :: playerCharacter is not valid!"), __LINE__);
		return;
	}

	// 장비 아이템 정보를 얻습니다.
	if (equipItemInfo == nullptr)
	{
		FString contextString;
		equipItemInfo = DT_EquipItemInfo->FindRow<FEquipItemInfo>(equippedItemSlotInfo->ItemCode, contextString);
	}

	// Skeletal Mesh 애셋 로드
	USkeletalMesh* loadedMesh = Cast<USkeletalMesh>(
		GetManager(FStreamableManager)->LoadSynchronous(equipItemInfo->MeshPath));

	USkeletalMesh* loadedSetMesh = nullptr;
	if (!equipItemInfo->SetMeshPath.IsNull())
	{
		loadedSetMesh = Cast<USkeletalMesh>(
			GetManager(FStreamableManager)->LoadSynchronous(equipItemInfo->SetMeshPath));
	}


	if (!IsValid(loadedMesh))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInventory.cpp :: %d LINE :: mesh path(%s) is not valid"),
			__LINE__, *equipItemInfo->MeshPath.ToString());
		return;
	}
	
	// 실제로 보여지는 Mesh 설정
	if (equipItemInfo->PartsType == EPartsType::PT_Glove)
	{
		playerCharacter->GetParts()[EPartsType::PT_RGlove]->SetSkeletalMesh(loadedMesh);
		playerCharacter->GetParts()[EPartsType::PT_LGlove]->SetSkeletalMesh(loadedSetMesh);
	}
	else playerCharacter->GetParts()[equipItemInfo->PartsType]->SetSkeletalMesh(loadedMesh);
}
