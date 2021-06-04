#include "NpcShopWnd.h"

#include "Single/PlayerManager/PlayerManager.h"
#include "Single/GameInstance/RPGGameInst.h"

#include "Actor/Controller/PlayerController/BasePlayerController.h"

#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"
#include "Widget/ClosableWnd/PlayerInventoryWnd/PlayerInventoryWnd.h"
#include "Widget/ClosableWnd/NpcShopWnd/ShopItemWidget/ShopItemWidget.h"
#include "Widget/BaseSlot/ItemSlot/PlayerInventoryItemSlot/PlayerInventoryItemSlot.h"
#include "Widget/ClosableWnd/TradeWnd/TradeWnd.h"
#include "Widget/ClosableWnd/MessageBoxWnd/MessageBoxWnd.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"

using namespace EMessageBoxButton;

UNpcShopWnd::UNpcShopWnd(const FObjectInitializer& objInitializer) : 
	Super(objInitializer)
{
	static ConstructorHelpers::FClassFinder<UShopItemWidget> BP_SHOP_ITEM(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/NpcShopWnd/BP_ShopItem.BP_ShopItem_C'"));
	if (BP_SHOP_ITEM.Succeeded()) BP_ShopItem = BP_SHOP_ITEM.Class;

	static ConstructorHelpers::FClassFinder<UTradeWnd> BP_TRADE_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/TradeWnd/BP_TradeWnd.BP_TradeWnd_C'"));
	if (BP_TRADE_WND.Succeeded()) BP_TradeWnd = BP_TRADE_WND.Class;
}

void UNpcShopWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// 창 크기 설정
	WndSize = FVector2D(665.0f, 600.0f);
}

UShopItemWidget* UNpcShopWnd::AddShopItemWidget(const FShopItemInfo& shopItemInfo)
{
	UShopItemWidget * shopItemWidget = CreateWidget<UShopItemWidget>(this, BP_ShopItem);
	
	GridPanel_ShopItems->AddChild(shopItemWidget);

	shopItemWidget->InitializeShopItemWidget(this, shopItemInfo);

	return shopItemWidget;
}

void UNpcShopWnd::InitializeNpcShop(FShopInfo* shopInfo)
{
	// 창 제목 설정
	SetTitleText(shopInfo->ShopName);

	const int maxColumnCount = 2;
	int currentColumnCount = 0;

	// 판매 아이템 추가
	for (auto shopItemInfo : shopInfo->ShopItems)
	{
		UShopItemWidget * shopItemWidget = AddShopItemWidget(shopItemInfo);

		UWidgetControllerWidget::SortGridPanelElem(shopItemWidget, maxColumnCount, currentColumnCount);
	}
}

void UNpcShopWnd::SaleItem(UPlayerInventoryWnd* playerInventoryWnd, UItemSlot* itemSlot)
{
	auto tradeWnd = CreateTradeWnd(ETradeSeller::Player, itemSlot);
	if (!IsValid(tradeWnd)) return;

	tradeWnd->OnTradeButtonClickedEvent.AddLambda(
		[this, playerInventoryWnd](UTradeWnd* tradeWnd)
		{
			auto inventorySlot = Cast<UPlayerInventoryItemSlot>(tradeWnd->GetConnectedItemSlot());
			int32 inputCount = tradeWnd->GetInputTradeCount();
			int32 itemPrice = inventorySlot->GetItemInfo()->Price * inputCount;

			UE_LOG(LogTemp, Warning, TEXT("(tradeWnd->IsInputTextEmpty() || inputCount == 0) = %d"),
				(tradeWnd->IsInputTextEmpty() || inputCount == 0));

			// 입력 값이 잘못 되었거나, 입력된 개수가 0인 경우
			if (tradeWnd->IsInputTextEmpty() || inputCount == 0)
			{
				auto msgBox = WidgetController->CreateMessageBox(
					/*titleText      = */ TEXT("입력 확인"),
					/*msg            = */ TEXT("입력된 내용이 잘못 되었다 닝겐"),
					/*bUseBackground = */ true,
					/*buttons        = */ MB_Ok);

				// Ok 버튼 이벤트 설정
				msgBox->OnOkButtonClickedEvent.BindLambda(
					[](UWidgetControllerWidget* widgetController, UMessageBoxWnd* msgBox)
					{
						msgBox->CloseThisWnd();
					});

				return;
			}

			// 아이템 판매
			// 아이템 이름을 얻습니다.
			FString itemName = inventorySlot->GetItemInfo()->ItemName.ToString();

			// 판매 개수를 얻습니다.
			FString tradeItemCount = FString::FromInt(tradeWnd->GetInputTradeCount());

			auto msgBox = WidgetController->CreateMessageBox(
				/*titleText      = */ TEXT("아이템 판매 확인"),
				/*msg            = */ itemName + TEXT("을(를) ") + tradeItemCount + TEXT("개 판매합니다."),
				/*bUseBackground = */ true,
				/*buttons        = */ MB_Ok | MB_Cancel);

			msgBox->OnOkButtonClickedEvent.BindLambda(
				[this, tradeWnd, playerInventoryWnd, itemPrice, inventorySlot, inputCount]
				(UWidgetControllerWidget* widgetController, UMessageBoxWnd* msgBox)
				{
					// 아이템 제거
					GetManager(UPlayerManager)->GetPlayerInventory()->RemoveItem(
						inventorySlot->GetItemSlotIndex(), inputCount);

					// 은화 획득
					GetManager(UPlayerManager)->GetPlayerInfo()->Silver += itemPrice;

					// 인벤토리 창 은화 갱신
					playerInventoryWnd->UpdateSilver();

					// 교환 창 닫기
					tradeWnd->CloseThisWnd();

					// 메시지 박스 닫기
					msgBox->CloseThisWnd();
				});

			msgBox->OnCancelButtonClickedEvent.BindLambda(
				[](UWidgetControllerWidget* widgetController, UMessageBoxWnd* msgBox)
				{
					msgBox->CloseThisWnd();
				});

		}
	);


}

UTradeWnd* UNpcShopWnd::CreateTradeWnd(
	ETradeSeller tradeSeller, 
	UItemSlot* connectedItemSlot, 
	FShopItemInfo* shopItemInfo)
{
	if (IsValid(TradeWnd)) return nullptr;
	if (connectedItemSlot->GetItemInfo()->IsEmpty()) return nullptr;

	TradeWnd = Cast<UTradeWnd>(GetManager(UPlayerManager)->GetPlayerController()->GetWidgetControllerWidget()->CreateWnd(
		BP_TradeWnd, true));

	// 교환 창 초기화
	TradeWnd->InitializeTradeWnd(tradeSeller, connectedItemSlot, shopItemInfo);

	TradeWnd->OnWndClosedEvent.AddLambda([this](UClosableWnd* closableWnd)
		{
			TradeWnd = nullptr;
		});

	return TradeWnd;
}

void UNpcShopWnd::FloatingInventoryWnd()
{
	auto widgetController = GetManager(UPlayerManager)->GetPlayerController()->GetWidgetControllerWidget();

	// 인벤토리 창을 생성합니다.
	auto playerInventoryWnd = GetManager(UPlayerManager)->GetPlayerInventory()->CreateInventoryWnd(
		widgetController, EInputModeType::IM_Default, true);

	// Npc 상점 창의 절반 크기를 얻습니다.
	FVector2D npcShopWndHalfSize = GetWndSize() * 0.5f;

	// npc 상점 창 위치를 얻습니다.
	FVector2D npcShopWndPos = GetCanvasPanelSlot()->GetPosition();

	// 인벤토리 창의 절반 크기를 얻습니다.
	FVector2D playerInventoryWndHalfSize = playerInventoryWnd->GetWndSize() * 0.5f;

	// 인벤토리 창의 위치를 계산합니다.
	FVector2D playerInventoryWndPos =
		npcShopWndPos + ((npcShopWndHalfSize + playerInventoryWndHalfSize) * FVector2D(1.0f, 0.0f));

	// 인벤토리 창과 상점 창의 높이를 맞춥니다.
	playerInventoryWndPos.Y += playerInventoryWndHalfSize.Y - npcShopWndHalfSize.Y;

	// 인벤토리 창의 위치를 설정합니다.
	playerInventoryWnd->GetCanvasPanelSlot()->SetPosition(playerInventoryWndPos);

	// 상점 창이 닫힐 때 인벤토리 창도 닫히도록 합니다.
	OnWndClosedEvent.AddLambda([this](class UClosableWnd* closableWnd)
		{ GetManager(UPlayerManager)->GetPlayerInventory()->CloseInventoryWnd(); });

	// 인벤토리 아이템 슬롯 우클릭 시 아이템 판매가 이루어질 수 있도록 합니다.
	for (auto slot : playerInventoryWnd->GetItemSlots())
	{
		slot->OnMouseRightButtonClicked.AddLambda(
			[this, slot, playerInventoryWnd](ESlotType itemSlotType)
			{ SaleItem(playerInventoryWnd, slot); });
	}




}
