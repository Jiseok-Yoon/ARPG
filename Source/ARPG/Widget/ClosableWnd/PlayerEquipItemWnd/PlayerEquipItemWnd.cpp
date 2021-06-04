#include "PlayerEquipItemWnd.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widget/BaseSlot/ItemSlot/PlayerEquipSlot/PlayerEquipSlot.h"

void UPlayerEquipItemWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// 창 크기 설정
	WndSize = FVector2D(500.0f, 800.0f);

	// 창비 아이템 슬롯 위젯 찾기
	{
		// 찾을 슬롯 위젯의 이름을 나타냅니다.
		TArray<FName> equipSlotNames = {
			FName(TEXT("BP_PlayerEquipSlot_Top")),
			FName(TEXT("BP_PlayerEquipSlot_Glove")),
			FName(TEXT("BP_PlayerEquipSlot_Bottom")),
			FName(TEXT("BP_PlayerEquipSlot_Shoes")),
			FName(TEXT("BP_PlayerEquipSlot_Weapon"))
		};

		// 슬롯 위젯 이름들을 이용하여 슬롯 위젯들을 찾습니다.
		for (FName slotWidgetName : equipSlotNames)
		{
			UPlayerEquipSlot* equipSlot = Cast<UPlayerEquipSlot>(GetWidgetFromName(slotWidgetName));
			EquipSlots.Add(equipSlot->GetPartsType(), equipSlot);
		}
	}

	// 장비 슬롯들을 갱신합니다.
	UpdatePartsSlot();
}

void UPlayerEquipItemWnd::UpdatePartsSlot()
{
	// 현재 장착중인 아이템들을 저장하는 컨테이너를 얻습니다.
	TMap<EPartsType, FItemSlotInfo>& currentPartsInfo = GetManager(UPlayerManager)->GetPlayerInfo()->PartsInfos;

	// 모든 슬롯들을 갱신합니다.
	for (auto equipSlot : EquipSlots)
	{
		// 현재 장착중인 장비 아이템중 equipSlot 과 같은 파츠에 대한 내용을 얻습니다.
		FItemSlotInfo& partsSlotInfo = currentPartsInfo[equipSlot.Key];

		equipSlot.Value->UpdateEquipSlot(partsSlotInfo);

	}

}
