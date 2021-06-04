#include "PlayerInventoryItemSlot.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Components/Image.h"

void UPlayerInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// 슬롯 드래깅이 시작되었을 경우
	OnSlotDragStarted.AddLambda(
		[this](USlotDragDropOperation* dragDropOp)
		{
			// 슬롯이 비어있다면 실행 취소
			if (GetItemInfo()->IsEmpty()) return;

			auto dragWidgetData = CreateSlotDragWidget();
			auto dragWidgetInstance = dragWidgetData.Get<0>();
			auto dragWidgetImage = dragWidgetData.Get<1>();

			// 드래그 시 표시되는 위젯을 설정합니다.
			dragDropOp->DefaultDragVisual = dragWidgetInstance;

			// 드래그 시 표시되는 이미지를 설정합니다.
			dragWidgetImage->SetBrushFromTexture(
				Cast<UTexture2D>(GetSlotImage()->Brush.GetResourceObject()));

			// 슬롯의 아이템 이미지 색상을 드래깅 색상으로 설정합니다.
			SetSlotColorDragging();
		});

	// 슬롯 드래깅이 취소되었을 경우
	OnSlotDragCancelled.AddLambda(
		[this](USlotDragDropOperation* dragDropOp)
		{
			// 슬롯이 비어있다면 실행 취소
			if (GetItemInfo()->IsEmpty()) return;

			// 아이템 이미지 색상을 기본 색상으로 설정합니다.
			SetSlotColorNormal();
		} );

	// 슬롯 드래깅이 성공적으로 끝났을 경우
	OnSlotDragFinished.AddLambda(
		[this](USlotDragDropOperation* dragDropOp)
		{
			auto originatedDragSlot = Cast<UPlayerInventoryItemSlot>(dragDropOp->OriginatedDragSlot);

			// 슬롯이 비어있다면 실행 취소
			if (originatedDragSlot->GetItemInfo()->IsEmpty()) return;

			// 아이템 이미지 색상을 기본 색상으로 설정합니다.
			dragDropOp->OriginatedDragSlot->SetSlotColorNormal();

			FPlayerCharacterInfo* playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();

			// 드래그를 시작시킨 슬롯과 드랍을 시킨 위치의 슬롯에 담긴 아이템이 동일한 아이템을 담고 있는지를 나타냅니다.
			bool bIsSameitem =
				playerInfo->InventoryItemInfos[originatedDragSlot->GetItemSlotIndex()].IsSameItem(
					playerInfo->InventoryItemInfos[GetItemSlotIndex()]);

			// 동일한 아이템이라면 아이템을 서로 합칩니다.
			if (bIsSameitem)
				GetManager(UPlayerManager)->GetPlayerInventory()->MergeItem(originatedDragSlot, this);

			// 동일한 아이템이 아니라면 인벤토리 슬롯 스왑
			else GetManager(UPlayerManager)->GetPlayerInventory()->SwapItem(originatedDragSlot, this);

		});

}

void UPlayerInventoryItemSlot::InitializeItemSlot(
	ESlotType slotType, FName itemCode, int32 itemSlotIndex)
{
	Super::InitializeSlot(slotType, itemCode);

	ItemSlotIndex = itemSlotIndex;

	UpdateItemCountText();
}

void UPlayerInventoryItemSlot::UpdateInventoryItemSlot()
{
	UpdateItemImage();
	UpdateItemCountText();
}

void UPlayerInventoryItemSlot::UpdateItemCountText()
{
	auto itemSlotInfo = GetManager(UPlayerManager)->GetPlayerInventory()->GetItemSlotInfo(ItemSlotIndex);

	SetSlotItemCount(itemSlotInfo->ItemCount);




}
