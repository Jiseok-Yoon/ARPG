#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Struct/ShopItemInfo/ShopItemInfo.h"

#include "ShopItemWidget.generated.h"

UCLASS()
class ARPG_API UShopItemWidget final : 
public UUserWidget
{
	GENERATED_BODY()

private :
	class UDataTable* DT_ItemInfo;

private :
	// Npc 상점 창 객체를 나타냅니다.
	class UNpcShopWnd* NpcShopWnd;

	// 상점 아이템 정보
	FShopItemInfo ShopItemInfo;

private :
	UPROPERTY(meta = (BindWidget))
	class UItemSlot * BP_ItemSlot;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Cost;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_ItemName;

public :
	UShopItemWidget(const FObjectInitializer& ObjectInitializer);

public :
	void InitializeShopItemWidget(class UNpcShopWnd* npcShopWnd, FShopItemInfo shopItemInfo);

protected :
	// 마우스 더블 클릭 입력이 있을 경우 호출됩니다.
	virtual FReply NativeOnMouseButtonDoubleClick(
		const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	/// - InGeometry : 위젯의 위치, 크기를 나타냅니다.
	/// - InMouseEvent : 입력 이벤트 종류
	
private :
	// 아이템을 구매합니다.
	void BuyItem();
};
