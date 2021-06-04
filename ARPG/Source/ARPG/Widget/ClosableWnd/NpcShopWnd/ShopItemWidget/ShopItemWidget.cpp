#include "ShopItemWidget.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Actor/Controller/PlayerController/BasePlayerController.h"

#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"
#include "Widget/ClosableWnd/NpcShopWnd/NpcShopWnd.h"
#include "Widget/BaseSlot/ItemSlot/ItemSlot.h"
#include "Widget/ClosableWnd/TradeWnd/TradeWnd.h"
#include "Widget/ClosableWnd/MessageBoxWnd/MessageBoxWnd.h"
#include "Widget/ClosableWnd/PlayerInventoryWnd/PlayerInventoryWnd.h"

#include "Components/TextBlock.h"


UShopItemWidget::UShopItemWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ItemInfo.DT_ItemInfo'"));
	if (DT_ITEM_INFO.Succeeded()) DT_ItemInfo = DT_ITEM_INFO.Object;
}

void UShopItemWidget::InitializeShopItemWidget(UNpcShopWnd* npcShopWnd, FShopItemInfo shopItemInfo)
{
	NpcShopWnd = npcShopWnd;
	ShopItemInfo = shopItemInfo;

	// ������ ���� �ʱ�ȭ
	BP_ItemSlot->InitializeSlot(ESlotType::ST_ShopItemSlot, shopItemInfo.ItemCode);

	// ������ �̸� ����
	Text_ItemName->SetText(BP_ItemSlot->GetItemInfo()->ItemName);

	// ������ ���� ����
	Text_Cost->SetText(FText::FromString(FString::FromInt(shopItemInfo.Cost)));
}

FReply UShopItemWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// ���� ���� Ŭ���� �̷�����ٸ�
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		// ������ ����
		BuyItem();

	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}

void UShopItemWidget::BuyItem()
{
	auto tradeWnd = NpcShopWnd->CreateTradeWnd(ETradeSeller::ShopKeeper, BP_ItemSlot, &ShopItemInfo);
	if (!IsValid(tradeWnd)) return;

	tradeWnd->OnTradeButtonClickedEvent.AddLambda(
		[this](UTradeWnd* tradeWnd)
		{
			// �Է� ���� �߸� �Ǿ��� ���
			if (tradeWnd->IsInputTextEmpty() || tradeWnd->GetInputTradeCount() == 0)
			{
				// �޽����ڽ� ����
				auto msgBox = GetManager(UPlayerManager)->GetPlayerController()->
					GetWidgetControllerWidget()->CreateMessageBox(
						TEXT("�Է� Ȯ��"),
						TEXT("�Էµ� ������ �߸� �Ǿ����ϴ�."),
						true,
						EMessageBoxButton::MB_Ok);

				// Ok ��ư�� Ŭ���Ǿ��� ��� �޽��� �ڽ��� �ݵ��� �մϴ�.
				msgBox->OnOkButtonClickedEvent.BindLambda(
					[](class UWidgetControllerWidget* widgetController, class UMessageBoxWnd* msgBox)
					{ msgBox->CloseThisWnd(); });

				return;
			}

			FString itemName = tradeWnd->GetConnectedItemSlot()->GetItemInfo()->ItemName.ToString();
			FString tradeItemCount = FString::FromInt(tradeWnd->GetInputTradeCount());

			// ���� Ȯ�� �޽��� �ڽ� ����
			auto msgBox = GetManager(UPlayerManager)->GetPlayerController()->
				GetWidgetControllerWidget()->CreateMessageBox(
					TEXT("������ ���� Ȯ��"),
					itemName + TEXT("��(��) ") + tradeItemCount + TEXT("�� �����մϴ�."),
					true,
					EMessageBoxButton::MB_Ok | EMessageBoxButton::MB_Cancel);

			msgBox->OnOkButtonClickedEvent.BindLambda(
				[this, tradeWnd](class UWidgetControllerWidget* widgetController, class UMessageBoxWnd* msgBox)
				{
					FString contextString;
					auto itemInfo = DT_ItemInfo->FindRow<FItemInfo>(
						ShopItemInfo.ItemCode, contextString);

					// �κ��丮�� �߰���ų ������ ������ �����մϴ�.
					FItemSlotInfo newItemSlotInfo(ShopItemInfo.ItemCode, tradeWnd->GetInputTradeCount(),
						((itemInfo->ItemType == EItemType::Equipment) ? 1 : itemInfo->MaxSlotCount));

					// �κ��丮�� �������� �߰��մϴ�.
					auto playerInventory = GetManager(UPlayerManager)->GetPlayerInventory();
					playerInventory->AddItem(newItemSlotInfo);

					// ������ ����
					int32 price = tradeWnd->GetInputTradeCount() * tradeWnd->GetShopItemInfo()->Cost;
					GetManager(UPlayerManager)->GetPlayerInfo()->Silver -= price;

					// �κ��丮 â�� ��ȭ ����
					auto playerInventoryWnd = playerInventory->GetPlayerInventoryWnd();
					if (IsValid(playerInventoryWnd)) playerInventoryWnd->UpdateSilver();

					tradeWnd->CloseThisWnd();
					msgBox->CloseThisWnd();
				});

			msgBox->OnCancelButtonClickedEvent.BindLambda(
				[](class UWidgetControllerWidget* widgetController, class UMessageBoxWnd* msgBox)
				{ msgBox->CloseThisWnd(); });
		}
	);
}
