#pragma once

#include "CoreMinimal.h"

#include "Enum/CharacterClassType.h"
#include "Enum/PartsType.h"

#include "Struct/ItemSlotInfo/ItemSlotInfo.h"
#include "Struct/QuestInfo/QuestInfo.h"
#include "Struct/ProgressQuestInfo/ProgressQuestInfo.h"

#include "PlayerCharacterInfo.generated.h"

USTRUCT()
struct ARPG_API FPlayerCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public :
	// 캐릭터 클래스 타입
	UPROPERTY()
	ECharacterClassType CharacterClass;

	// 캐릭터 레벨
	UPROPERTY()
	int32 CharacterLevel;

	// 인벤토리 슬롯 개수
	UPROPERTY()
	int32 InventorySlotCount;

	// 소지중인 아이템 정보
	UPROPERTY()
	TArray<FItemSlotInfo> InventoryItemInfos;

	// 기본 장착 아이템 정보
	UPROPERTY()
	TMap<EPartsType, FItemSlotInfo> DefaultPartsInfos;

	// 장착 아이템 정보
	UPROPERTY()
	TMap<EPartsType, FItemSlotInfo> PartsInfos;

	// 소지금
	UPROPERTY()
	int32 Silver;

	// 진행중인 퀘스트 목록
	TArray<FProgressQuestInfo> ProgressQuestInfos;

	// 완료한 퀘스트 코드 목록
	TArray<FName> CompletedQuestCodes;

public :
	FPlayerCharacterInfo();

	// 기본 장비 아이템들 초기화
	void InitializeDefaultEquipmentItems(
		TArray<FName>& defaultEquipmentItemCodes, const class UDataTable* dt_EquipItemInfo);

	// 장비 아이템 정보를 얻습니다.
	/// - Return : <장비 아이템 슬롯 정보, 반환된 슬롯 정보가 기본 장비 아이템 정보임>
	TTuple<FItemSlotInfo, bool> GetEquippedItemSlotInfo(EPartsType partsType, bool* out_bReturnedAvailableData = nullptr) const;

	// 모든 장비 아이템 정보를 얻습니다.
	TMap< EPartsType, TTuple<FItemSlotInfo, bool> > GetAllEquippedItemSlotInfo() const;

	// 진행 퀘스트를 추가합니다.
	void AddProgressQuest(FName newQuestCode, FQuestInfo newQuestInfo);


};
