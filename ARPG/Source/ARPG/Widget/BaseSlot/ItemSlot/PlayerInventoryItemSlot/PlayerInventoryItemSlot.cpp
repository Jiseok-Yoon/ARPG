#include "PlayerInventoryItemSlot.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Components/Image.h"

void UPlayerInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� �巡���� ���۵Ǿ��� ���
	OnSlotDragStarted.AddLambda(
		[this](USlotDragDropOperation* dragDropOp)
		{
			// ������ ����ִٸ� ���� ���
			if (GetItemInfo()->IsEmpty()) return;

			auto dragWidgetData = CreateSlotDragWidget();
			auto dragWidgetInstance = dragWidgetData.Get<0>();
			auto dragWidgetImage = dragWidgetData.Get<1>();

			// �巡�� �� ǥ�õǴ� ������ �����մϴ�.
			dragDropOp->DefaultDragVisual = dragWidgetInstance;

			// �巡�� �� ǥ�õǴ� �̹����� �����մϴ�.
			dragWidgetImage->SetBrushFromTexture(
				Cast<UTexture2D>(GetSlotImage()->Brush.GetResourceObject()));

			// ������ ������ �̹��� ������ �巡�� �������� �����մϴ�.
			SetSlotColorDragging();
		});

	// ���� �巡���� ��ҵǾ��� ���
	OnSlotDragCancelled.AddLambda(
		[this](USlotDragDropOperation* dragDropOp)
		{
			// ������ ����ִٸ� ���� ���
			if (GetItemInfo()->IsEmpty()) return;

			// ������ �̹��� ������ �⺻ �������� �����մϴ�.
			SetSlotColorNormal();
		} );

	// ���� �巡���� ���������� ������ ���
	OnSlotDragFinished.AddLambda(
		[this](USlotDragDropOperation* dragDropOp)
		{
			auto originatedDragSlot = Cast<UPlayerInventoryItemSlot>(dragDropOp->OriginatedDragSlot);

			// ������ ����ִٸ� ���� ���
			if (originatedDragSlot->GetItemInfo()->IsEmpty()) return;

			// ������ �̹��� ������ �⺻ �������� �����մϴ�.
			dragDropOp->OriginatedDragSlot->SetSlotColorNormal();

			FPlayerCharacterInfo* playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();

			// �巡�׸� ���۽�Ų ���԰� ����� ��Ų ��ġ�� ���Կ� ��� �������� ������ �������� ��� �ִ����� ��Ÿ���ϴ�.
			bool bIsSameitem =
				playerInfo->InventoryItemInfos[originatedDragSlot->GetItemSlotIndex()].IsSameItem(
					playerInfo->InventoryItemInfos[GetItemSlotIndex()]);

			// ������ �������̶�� �������� ���� ��Ĩ�ϴ�.
			if (bIsSameitem)
				GetManager(UPlayerManager)->GetPlayerInventory()->MergeItem(originatedDragSlot, this);

			// ������ �������� �ƴ϶�� �κ��丮 ���� ����
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
