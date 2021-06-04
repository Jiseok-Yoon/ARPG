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

	// 아이템 슬롯 초기화
	BP_ItemSlot->InitializeSlot(ESlotType::ST_ShopItemSlot, shopItemInfo.ItemCode);

	// 아이템 이름 설정
	Text_ItemName->SetText(BP_ItemSlot->GetItemInfo()->ItemName);

	// 아이템 가격 설정
	Text_Cost->SetText(FText::FromString(FString::FromInt(shopItemInfo.Cost)));
}

FReply UShopItemWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 왼쪽 더블 클릭이 이루어졌다면
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		// 아이템 구매
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
			// 입력 값이 잘못 되었을 경우
			if (tradeWnd->IsInputTextEmpty() || tradeWnd->GetInputTradeCount() == 0)
			{
				// 메시지박스 띄우기
				auto msgBox = GetManager(UPlayerManager)->GetPlayerController()->
					GetWidgetControllerWidget()->CreateMessageBox(
						TEXT("입력 확인"),
						TEXT("입력된 내용이 잘못 되었습니다."),
						true,
						EMessageBoxButton::MB_Ok);

				// Ok 버튼이 클릭되었을 경우 메시지 박스를 닫도록 합니다.
				msgBox->OnOkButtonClickedEvent.BindLambda(
					[](class UWidgetControllerWidget* widgetController, class UMessageBoxWnd* msgBox)
					{ msgBox->CloseThisWnd(); });

				return;
			}

			FString itemName = tradeWnd->GetConnectedItemSlot()->GetItemInfo()->ItemName.ToString();
			FString tradeItemCount = FString::FromInt(tradeWnd->GetInputTradeCount());

			// 구매 확인 메시지 박스 띄우기
			auto msgBox = GetManager(UPlayerManager)->GetPlayerController()->
				GetWidgetControllerWidget()->CreateMessageBox(
					TEXT("아이템 구매 확인"),
					itemName + TEXT("을(를) ") + tradeItemCount + TEXT("개 구매합니다."),
					true,
					EMessageBoxButton::MB_Ok | EMessageBoxButton::MB_Cancel);

			msgBox->OnOkButtonClickedEvent.BindLambda(
				[this, tradeWnd](class UWidgetControllerWidget* widgetController, class UMessageBoxWnd* msgBox)
				{
					FString contextString;
					auto itemInfo = DT_ItemInfo->FindRow<FItemInfo>(
						ShopItemInfo.ItemCode, contextString);

					// 인벤토리에 추가시킬 아이템 정보를 생성합니다.
					FItemSlotInfo newItemSlotInfo(ShopItemInfo.ItemCode, tradeWnd->GetInputTradeCount(),
						((itemInfo->ItemType == EItemType::Equipment) ? 1 : itemInfo->MaxSlotCount));

					// 인벤토리에 아이템을 추가합니다.
					auto playerInventory = GetManager(UPlayerManager)->GetPlayerInventory();
					playerInventory->AddItem(newItemSlotInfo);

					// 소지금 감소
					int32 price = tradeWnd->GetInputTradeCount() * tradeWnd->GetShopItemInfo()->Cost;
					GetManager(UPlayerManager)->GetPlayerInfo()->Silver -= price;

					// 인벤토리 창의 은화 갱신
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
