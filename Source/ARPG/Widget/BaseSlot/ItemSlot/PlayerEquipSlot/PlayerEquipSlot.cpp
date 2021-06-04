#include "PlayerEquipSlot.h"
#include "Components/TextBlock.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widget/BaseSlot/ItemSlot/PlayerInventoryItemSlot/PlayerInventoryItemSlot.h"
#include "Widget/Closablewnd/PlayerInventoryWnd/PlayerInventoryWnd.h"

#include "Components/Image.h"

void UPlayerEquipSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// 슬롯 드래그 허용
	bAllowDragOperation = true;

	SlotType = ESlotType::ST_EquipSlot;

	// 드래그 시작 시 실행할 내용을 정의합니다.
	OnSlotDragStarted.AddLambda(
		[this](USlotDragDropOperation* dragDropOp)
		{
			// 슬롯이 비어있다면 실행 취소
			if (GetItemInfo()->IsEmpty()) return;

			auto dragWidgetData = CreateSlotDragWidget();
			dragDropOp->DefaultDragVisual = dragWidgetData.Get<0>();
			auto dragWidgetImage = dragWidgetData.Get<1>();

			// 드래그 시 표시되는 이미지를 설정합니다.
			dragWidgetImage->SetBrushFromTexture(
				Cast<UTexture2D>(GetSlotImage()->Brush.GetResourceObject()));

			// 아이템 이미지를 드래깅 색상으로 설정합니다.
			SetSlotColorDragging();
		});

	// 드래그 취소 시 실행할 내용을 정의합니다.
	OnSlotDragCancelled.AddLambda(
		[this](USlotDragDropOperation* dragDropOp)
		{
			// 슬롯이 비어있다면 실행 X
			if (GetItemInfo()->IsEmpty()) return;

			// 아이템 이미지를 기본 색상으로 설정합니다.
			SetSlotColorNormal();
		});

	OnSlotDragFinished.AddLambda(
		[this](USlotDragDropOperation* slotDragDropOp)
		{
			// 인벤토리 슬롯이 드래그 드랍 되었다면
			if (slotDragDropOp->OriginatedDragSlot->GetSlotType() == ESlotType::ST_InventorySlot)
			{
				// 드래깅된 인벤토리 슬롯을 얻습니다.
				auto originatedSlot = Cast<UPlayerInventoryItemSlot>(slotDragDropOp->OriginatedDragSlot);

				originatedSlot->AttachToEquipItemSlot(this);

				// 아이템 이미지 색상을 기본 색상으로 설정합니다.
				originatedSlot->SetSlotColorNormal();

				SetSlotColorNormal();
			}

		});

	// 우클릭, 좌 더블 클릭 시 장비 아이템을 장착 해제시킵니다.
	OnMouseRightButtonClicked.AddLambda([this](ESlotType slotType)  { DetachToEquipItemSlot(); });
	OnMouseLeftButtonDBClicked.AddLambda([this](ESlotType slotType) { DetachToEquipItemSlot(); });

}

void UPlayerEquipSlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	{
		FString partsText;

		switch (PartsType)
		{
		case EPartsType::PT_Top:
			partsText = TEXT("상의");
			break;
		case EPartsType::PT_Bottom:
			partsText = TEXT("하의");
			break;
		case EPartsType::PT_Glove:
			partsText = TEXT("장갑");
			break;
		case EPartsType::PT_Shoes:
			partsText = TEXT("신발");
			break;
		case EPartsType::PT_Weapon:
			partsText = TEXT("무기");
			break;

		case EPartsType::PT_Hair:
		case EPartsType::PT_Head:
		case EPartsType::PT_LGlove:
		case EPartsType::PT_RGlove:
		case EPartsType::PT_Beard:
		default:
			partsText = TEXT("");
			break;
		}

		Text_PartsName->SetText(FText::FromString(partsText));
	}
}

void UPlayerEquipSlot::UpdateEquipSlot(FItemSlotInfo& itemSlotInfo)
{
	SetItemInfo(itemSlotInfo.ItemCode);
	UpdateItemImage();

}

void UPlayerEquipSlot::DetachToEquipItemSlot(int32 inventorySlotIndex)
{
	// 슬롯이 비어있다면 실행 X
	if (GetItemInfo()->IsEmpty()) return;

	UPlayerInventory * playerInventory = GetManager(UPlayerManager)->GetPlayerInventory();

	// 전에 장착했던 장비 아이템 정보를 저장할 변수
	FItemSlotInfo prevEquippedItemInfo;

	// 장비 아이템 장착 해제
	playerInventory->DismountItem(PartsType, &prevEquippedItemInfo);
	
	if (inventorySlotIndex != INDEX_NONE)
	{
		inventorySlotIndex = playerInventory->GetItemSlotInfo(inventorySlotIndex)->IsEmpty() ? 
			inventorySlotIndex : INDEX_NONE;
	}

	// inventorySlotIndex 이 INDEX_NONE 이거나, inventorySlotIndex 번째 인벤토리 슬롯에 아이템을 추가할 수 없는 상태라면
	// 추가 가능한 첫 번째 슬롯에 아이템을 추가합니다.
	if (inventorySlotIndex == INDEX_NONE)
		// 착용중이던 장비 아이템을 인벤토리에 추가합니다.
		playerInventory->AddItem(prevEquippedItemInfo);

	// inventorySlotIndex 번째 슬롯이 비어있다면
	else
	{
		// inventorySlotIndex 번째 슬롯에 이전에 장착했던 장비 아이템을 추가합니다.
		GetManager(UPlayerManager)->GetPlayerInfo()->InventoryItemInfos[inventorySlotIndex] =
			prevEquippedItemInfo;

		// 인벤토리 창에 추가된 아이템 슬롯들을 갱신합니다.
		if (IsValid(playerInventory->GetPlayerInventoryWnd()))
			playerInventory->GetPlayerInventoryWnd()->UpdateInventoryItemSlots();
	}

	// 장비 슬롯 갱신
	UpdateEquipSlot(GetManager(UPlayerManager)->GetPlayerInfo()->PartsInfos[PartsType]);
}
