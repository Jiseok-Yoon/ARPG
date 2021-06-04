#pragma once

#include "CoreMinimal.h"
#include "Widget/ClosableWnd/ClosableWnd.h"

#include "Struct/ShopItemInfo/ShopItemInfo.h"

#include "Enum/TradeSeller.h"
#include "TradeWnd.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTradeWndButtonSignature, class UTradeWnd *)

UCLASS()
class ARPG_API UTradeWnd : public UClosableWnd
{
	GENERATED_BODY()

public :
	// 구매 / 판매 버튼이 클릭되었을 경우 호출되는 대리자
	FTradeWndButtonSignature OnTradeButtonClickedEvent;

private :
	// 연결된 슬롯 객체를 나타냅니다.
	class UItemSlot* ConnectedItemSlot;

	// 판매자를 나타냅니다.
	ETradeSeller TradeSeller;

	// 판매 아이템 정보
	FShopItemInfo* ShopItemInfo;

private :
	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_ItemCost;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_TradeButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox * EditableTextBox_TradeCount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Result;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Trade;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Cancel;

protected :
	virtual void NativeConstruct() override;

public :
	// 교환 창을 초기화합니다.
	void InitializeTradeWnd(
		ETradeSeller tradeSeller,
		class UItemSlot* connectedItemSlot,
		FShopItemInfo* shopItemInfo = nullptr);

private  :
	UFUNCTION()
	void OnTradeButtonClicked();

	UFUNCTION()
	void OnTradeCountTextChanged(const FText& Text);

public :
	// 입력한 문자열이 비어있는지 확인합니다.
	bool IsInputTextEmpty() const;

	// 입력한 개수를 int32 형식으로 반환합니다.
	int32 GetInputTradeCount() const;

	FORCEINLINE class UItemSlot* GetConnectedItemSlot() const
	{ return ConnectedItemSlot; }

	FORCEINLINE FShopItemInfo* GetShopItemInfo() const
	{ return ShopItemInfo; }
};
