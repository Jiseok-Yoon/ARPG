#pragma once

#include "CoreMinimal.h"
#include "Widget/ClosableWnd/ClosableWnd.h"
#include "Enum/PartsType.h"
#include "PlayerEquipItemWnd.generated.h"

UCLASS()
class ARPG_API UPlayerEquipItemWnd : public UClosableWnd
{
	GENERATED_BODY()

private :
	// 창에 표시되는 슬롯들을 나타냅니다.
	TMap<EPartsType, class UPlayerEquipSlot*> EquipSlots;

protected :
	virtual void NativeConstruct() override;

public :
	// 파츠 슬롯들을 갱신합니다.
	void UpdatePartsSlot();

public :
	FORCEINLINE class UPlayerEquipSlot* GetPlayerEquipSlot(EPartsType partsType)
	{
		if (!EquipSlots.Contains(partsType)) return nullptr;
		return EquipSlots[partsType];
	}

	
};
