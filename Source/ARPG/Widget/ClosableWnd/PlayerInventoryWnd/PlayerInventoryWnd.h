#pragma once

#include "ARPG.h"
#include "Widget/ClosableWnd/ClosableWnd.h"
#include "PlayerInventoryWnd.generated.h"

UCLASS()
class ARPG_API UPlayerInventoryWnd : public UClosableWnd
{
	GENERATED_BODY()

private :
	TSubclassOf<class UPlayerInventoryItemSlot> BP_PlayerInventoryItemSlot;

private :
	TArray<class UPlayerInventoryItemSlot*> ItemSlots;

private :
	UPROPERTY(meta = (BindWidget))
	class UGridPanel* GridPanel_ItemSlots;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Silver;

public :
	UPlayerInventoryWnd(const FObjectInitializer& ObjectInitializer);

protected :
	virtual void NativeConstruct() override;

private :
	void InitializeInventoryWnd();

	// 아이템 슬롯을 생성합니다.
	class UPlayerInventoryItemSlot* CreateItemSlot();

public :
	// 인벤토리 아이템 슬롯들을 갱신합니다.
	void UpdateInventoryItemSlots();

	// 소지금을 갱신합니다.
	void UpdateSilver();

public :
	TArray<class UPlayerInventoryItemSlot*>& GetItemSlots()
	{ return ItemSlots; }


	
};
