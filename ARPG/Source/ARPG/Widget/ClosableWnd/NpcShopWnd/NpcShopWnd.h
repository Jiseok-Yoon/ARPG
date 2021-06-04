#pragma once

#include "CoreMinimal.h"
#include "Widget/ClosableWnd/ClosableWnd.h"

#include "Struct/ShopInfo/ShopInfo.h"

#include "Enum/TradeSeller.h"
#include "NpcShopWnd.generated.h"

UCLASS()
class ARPG_API UNpcShopWnd final : 
	public UClosableWnd
{
	GENERATED_BODY()

private :
	TSubclassOf<class UShopItemWidget> BP_ShopItem;
	TSubclassOf<class UTradeWnd> BP_TradeWnd;

private :
	UPROPERTY()
	class UTradeWnd* TradeWnd;

private :
	UPROPERTY(meta = (BindWidget))
	class UGridPanel* GridPanel_ShopItems;

public :
	UNpcShopWnd(const FObjectInitializer& objInitializer);

protected :
	virtual void NativeConstruct() override;

private :
	// 판매 아이템을 추가합니다.
	/// - shopItemInfo : 판매 아이템 정보를 전달합니다.
	class UShopItemWidget* AddShopItemWidget(const FShopItemInfo& shopItemInfo);

public :
	// 상점 창을 초기화합니다.
	/// - shopInfo : 상점 정보를 전달합니다.
	void InitializeNpcShop(FShopInfo* shopInfo);

	// 아이템을 판매합니다.
	void SaleItem(class UPlayerInventoryWnd* playerInventoryWnd, class UItemSlot* itemSlot);

	// 아이템 교환 창을 생성합니다.
	class UTradeWnd* CreateTradeWnd(ETradeSeller tradeSeller, class UItemSlot * connectedItemSlot, FShopItemInfo * shopItemInfo = nullptr);

public :
	// 인벤토리 창을 상점 창 우측에 띄웁니다.
	UFUNCTION()
	void FloatingInventoryWnd();
	
};
