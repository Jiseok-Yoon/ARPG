#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseSlot/ItemSlot/ItemSlot.h"
#include "Struct/ItemSlotInfo/ItemSlotInfo.h"

#include "Enum/PartsType.h"
#include "PlayerEquipSlot.generated.h"

UCLASS()
class ARPG_API UPlayerEquipSlot : public UItemSlot
{
	GENERATED_BODY()

private :
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PartsName;

private :
	// 슬롯이 표시하는 파츠를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPartsType PartsType;

protected :
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

public :
	void UpdateEquipSlot(FItemSlotInfo& itemSlotInfo);

	// 장비 아이템 장착 해제시킵니다.
	/// - inventorySlotIndex : 장착중이던 장비 아이템을 어느 인벤토리 슬롯에 추가할 것인지를 지정합니다.
	void DetachToEquipItemSlot(int32 inventorySlotIndex = INDEX_NONE);

public :
	FORCEINLINE EPartsType GetPartsType() const
	{ return PartsType; }




};
