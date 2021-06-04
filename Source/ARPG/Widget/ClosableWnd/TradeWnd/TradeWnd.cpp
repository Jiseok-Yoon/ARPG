#include "TradeWnd.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widget/BaseSlot/ItemSlot/PlayerInventoryItemSlot/PlayerInventoryItemSlot.h"
#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"
#include "Widget/ClosableWnd/MessageBoxWnd/MessageBoxWnd.h"

#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

using namespace EMessageBoxButton;

void UTradeWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// 창 크기 설정
	WndSize = FVector2D(600.0f, 320.0f);

	EditableTextBox_TradeCount->OnTextChanged.AddDynamic(this, &UTradeWnd::OnTradeCountTextChanged);
	Button_Trade->OnClicked.AddDynamic(this, &UTradeWnd::OnTradeButtonClicked);
	Button_Cancel->OnClicked.AddDynamic(this, &UTradeWnd::CloseThisWnd);
}

void UTradeWnd::InitializeTradeWnd(
	ETradeSeller tradeSeller, 
	UItemSlot* connectedItemSlot, 
	FShopItemInfo* shopItemInfo)
{
	// 창 이름을 설정합니다.
	SetTitleText(connectedItemSlot->GetItemInfo()->ItemName);

	// 슬롯 연결
	ConnectedItemSlot = connectedItemSlot;

	// 판매 아이템 정보 설정
	ShopItemInfo = shopItemInfo;

	// 판매자 설정
	TradeSeller = tradeSeller;

	switch (TradeSeller)
	{
	case ETradeSeller::ShopKeeper :
		// 아이템 가격 텍스트 설정
		Text_ItemCost->SetText(FText::FromString(FString::FromInt(shopItemInfo->Cost)));
		break;

	case ETradeSeller::Player :
		// 아이템 가격 텍스트 설정
		Text_ItemCost->SetText(FText::FromString(FString::FromInt(connectedItemSlot->GetItemInfo()->Price)));
		break;
	}

	// 버튼 텍스트 설정
	Text_TradeButton->SetText(FText::FromString(
		(TradeSeller == ETradeSeller::ShopKeeper ? TEXT("구매") : TEXT("판매"))
	));

	

}

void UTradeWnd::OnTradeButtonClicked()
{
	if (OnTradeButtonClickedEvent.IsBound())
		OnTradeButtonClickedEvent.Broadcast(this);
}

void UTradeWnd::OnTradeCountTextChanged(const FText& Text)
{
	// EditableTextBox 의 내용을 int32 형식 데이터로 설정합니다.
	Action(fnSetEditTextContentFromInt, (UEditableTextBox*, int32)) =
		[](UEditableTextBox* editableTextBox, int32 value)
	{ editableTextBox->SetText(FText::FromString(FString::FromInt(value))); };

	// TextBlock 의 내용을 int32 형식 데이터로 설정합니다.
	Action(fnSetTextBlockContentFromInt, (UTextBlock *, int32)) = 
		[](UTextBlock* textBlock, int32 value)
	{ textBlock->SetText(FText::FromString(FString::FromInt(value))); };
	
	// 전달된 FText 데이터를 숫자로 변경하여 반환합니다.
	Func(int32, fnTextToInt, (const FText&)) =
		[](const FText& fromText)
	{
		return FCString::Atoi(*fromText.ToString());
		/// - FCString : C Style 의 문자열과 함께 사용되는 함수들을 제공하는 구조체
	};

	// 입력되어있는 문자열이 비어있지 않으며, 숫자가 아니라면
	if (!Text.IsNumeric())
	{
		EditableTextBox_TradeCount->SetText(FText());
		fnSetTextBlockContentFromInt(Text_Result, 0);
		return;
	}

	// 입력된 문자열을 숫자로 변경하여 저장합니다.
	int32 tradeCount = fnTextToInt(Text);

	// 교환 가능한 최대 아이템 개수를 저장합니다.
	int32 maxTradeCount = 0;

	// 아이템 가격을 나타냅니다.
	int32 price = 0;

	switch (TradeSeller)
	{
	case ETradeSeller::ShopKeeper :
		{
			// 소지금
			int32 silver = GetManager(UPlayerManager)->GetPlayerInfo()->Silver;

			// 아이템 가격 저장
			price = ShopItemInfo->Cost;

			// 최대 구매 가능한 아이템 개수를 최대로 구매 가능한 아이템 개수로 설정합니다.
			maxTradeCount = (int32)(silver / price);

			// 장비 아이템이라면 최대 구매 가능 개수를 1 로 설정합니다.
			if (ConnectedItemSlot->GetItemInfo()->ItemType == EItemType::Equipment)
				if (maxTradeCount != 0) maxTradeCount = 1;

			break;
		}

	case ETradeSeller::Player :
		{
			// 판매하려는 아이템 슬롯 인덱스를 얻습니다.
			int32 slotIndex = Cast<UPlayerInventoryItemSlot>(ConnectedItemSlot)->GetItemSlotIndex();

			// 판매하려는 아이템 슬롯 정보를 얻습니다.
			auto slotInfo = GetManager(UPlayerManager)->GetPlayerInfo()->InventoryItemInfos[slotIndex];

			// 아이템 가격을 얻습니다.
			price = ConnectedItemSlot->GetItemInfo()->Price;

			// 최대 판매 가능한 아이템 개수를 슬롯의 아이템 개수로 설정합니다.
			maxTradeCount = slotInfo.ItemCount;
			break;
		}
	}

	// 최대 교환 개수를 초과했다면 입력된 숫자를 최대 교환 가능 개수로 설정합니다.
	if (tradeCount > maxTradeCount) tradeCount = maxTradeCount;

	// 계산된 개수로 설정합니다.
	fnSetEditTextContentFromInt(EditableTextBox_TradeCount, tradeCount);

	// 가격을 설정합니다.
	fnSetTextBlockContentFromInt(Text_Result, price * tradeCount);

}

bool UTradeWnd::IsInputTextEmpty() const
{
	return EditableTextBox_TradeCount->GetText().IsEmpty();
}

int32 UTradeWnd::GetInputTradeCount() const
{
	return FCString::Atoi(*EditableTextBox_TradeCount->GetText().ToString());
}

