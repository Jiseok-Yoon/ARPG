#include "PlayerCharacterInfo.h"

#include "Struct/EquipItemInfo/EquipItemInfo.h"

FPlayerCharacterInfo::FPlayerCharacterInfo()
{
	// 캐릭터 클래스 초기화
	/// - -TODO- 캐릭터가 추가되면 제거되어야 하는 구문
	CharacterClass = ECharacterClassType::CLS_Warrior;

	InventorySlotCount = 50;

	for (int32 i = 0; i < InventorySlotCount; ++i)
		InventoryItemInfos.Add(FItemSlotInfo());

	// 기본 장비 정보 설정
	DefaultPartsInfos.Add(EPartsType::PT_Head, FItemSlotInfo());
	DefaultPartsInfos.Add(EPartsType::PT_Hair, FItemSlotInfo());
	DefaultPartsInfos.Add(EPartsType::PT_Beard, FItemSlotInfo());
	DefaultPartsInfos.Add(EPartsType::PT_Top, FItemSlotInfo());
	DefaultPartsInfos.Add(EPartsType::PT_Bottom, FItemSlotInfo());
	DefaultPartsInfos.Add(EPartsType::PT_Glove, FItemSlotInfo());
	DefaultPartsInfos.Add(EPartsType::PT_Shoes, FItemSlotInfo());
	DefaultPartsInfos.Add(EPartsType::PT_Weapon, FItemSlotInfo());


	// 각 파츠 소켓 생성
	PartsInfos.Add(EPartsType::PT_Head, FItemSlotInfo());
	PartsInfos.Add(EPartsType::PT_Hair, FItemSlotInfo());
	PartsInfos.Add(EPartsType::PT_Beard, FItemSlotInfo());
	PartsInfos.Add(EPartsType::PT_Top, FItemSlotInfo());
	PartsInfos.Add(EPartsType::PT_Bottom, FItemSlotInfo());
	PartsInfos.Add(EPartsType::PT_Glove, FItemSlotInfo());
	PartsInfos.Add(EPartsType::PT_Shoes, FItemSlotInfo());
	PartsInfos.Add(EPartsType::PT_Weapon, FItemSlotInfo());

	// TEST
	Silver = 10000;
	InventoryItemInfos[1] = (FItemSlotInfo(FName(TEXT("12001")), 1));
	InventoryItemInfos[12] = (FItemSlotInfo(FName(TEXT("12002")), 1));




}

void FPlayerCharacterInfo::InitializeDefaultEquipmentItems(
	TArray<FName>& defaultEquipmentItemCodes, 
	const UDataTable* dt_EquipItemInfo)
{
	for (FName defaultEquipItemCode : defaultEquipmentItemCodes)
	{
		// 기본 장비 아이템 정보를 얻습니다.
		FString contextString;
		FEquipItemInfo* defaultEquipmentItemInfo = 
			dt_EquipItemInfo->FindRow<FEquipItemInfo>(defaultEquipItemCode, contextString);

		// 아이템 정보를 찾지 못했다면
		if (defaultEquipmentItemInfo == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerCharacterInfo.cpp :: %d LINE :: defaultEquipItemCode(%s) is not valid!"),
				__LINE__, *defaultEquipItemCode.ToString());
			continue;
		}

		DefaultPartsInfos[defaultEquipmentItemInfo->PartsType] = FItemSlotInfo(defaultEquipItemCode);
	}

}

TTuple<FItemSlotInfo, bool> FPlayerCharacterInfo::GetEquippedItemSlotInfo(
	EPartsType partsType, 
	bool* out_bReturnedAvailableData) const
{
	// Map 에 추가된 요소인지 확인
	/// - PartsInfos 와 DefaultPartsInfos 에 key 가 partsType 으로 된 요소가 존재하지 않는다면 정상적인 리턴 값이 아님.
	/// - partsType 에 PT_LGlove 와 PT_RGlove 가 전달되어 이 메서드가 작동하는 것을 방지하기 위해 작성됨
	if (!PartsInfos.Contains(partsType) && !DefaultPartsInfos.Contains(partsType))
	{
		UE_LOG(LogTemp, Error, TEXT("FPlayerCharacterInfo.cpp :: %d LINE :: (EPartsType)[%d] is not AvailablePartsType!"),
			__LINE__, static_cast<uint8>(partsType));

		if (out_bReturnedAvailableData)
			*out_bReturnedAvailableData = false;

		return MakeTuple(FItemSlotInfo(), false);
	}

	// 기본 장비 정보
	FItemSlotInfo defaultEquipItemInfo = DefaultPartsInfos[partsType];

	// 장착된 장비 정보
	FItemSlotInfo equippedItemSlotInfo = PartsInfos[partsType];

	// 장착된 장비가 없다면
	if (equippedItemSlotInfo.IsEmpty())
	{
		if (out_bReturnedAvailableData)
			*out_bReturnedAvailableData = true;

		// 기본 장비 정보 반환
		return MakeTuple(defaultEquipItemInfo, true);
	}

	// 장착된 장비가 있다면
	else
	{
		if (out_bReturnedAvailableData)
			*out_bReturnedAvailableData = true;

		// 장착된 장비 정보 반환
		return MakeTuple(equippedItemSlotInfo, false); 
	}
}

TMap<EPartsType, TTuple<FItemSlotInfo, bool>> FPlayerCharacterInfo::GetAllEquippedItemSlotInfo() const
{
	TMap<EPartsType, TTuple<FItemSlotInfo, bool>> returnValue;

	// PartsInfos 에 요소 할당이 이루어지지 않았는지 확인합니다.
	if (PartsInfos.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacterInfo.cpp :: %d LINE :: PartsInfos,Num() is zero!"),
			__LINE__);
	}
	else
	{
		// 찾고자 하는 파츠 타입들을 배열에 저장합니다.
		TArray<EPartsType> partsKeys;
		{
			partsKeys.Add(EPartsType::PT_Hair);
			partsKeys.Add(EPartsType::PT_Head);
			partsKeys.Add(EPartsType::PT_Beard);
			partsKeys.Add(EPartsType::PT_Top);
			partsKeys.Add(EPartsType::PT_Bottom);
			partsKeys.Add(EPartsType::PT_Glove);
			//partsKeys.Add(EPartsType::PT_LGlove);
			//partsKeys.Add(EPartsType::PT_RGlove);
			partsKeys.Add(EPartsType::PT_Shoes);
			partsKeys.Add(EPartsType::PT_Weapon);
		}

		for (EPartsType partsKey : partsKeys)
		{
			// 현재 장착된 장비 아이템 슬롯 정보를 얻습니다.
			bool bReturnedAvailableData = false; // 반환된 데이터가 유효한 데이터임을 확인하기 위한 변수
			TTuple<FItemSlotInfo, bool> equippedItemSlotInfo =
				GetEquippedItemSlotInfo(partsKey, &bReturnedAvailableData);

			// 정상적인 정보가 아니라면 실행 X
			if (!bReturnedAvailableData) continue;

			// 요소 추가
			returnValue.Add(partsKey, equippedItemSlotInfo);
		}
	}

	return returnValue;
}

void FPlayerCharacterInfo::AddProgressQuest(FName newQuestCode, FQuestInfo newQuestInfo)
{
	ProgressQuestInfos.Add(FProgressQuestInfo(newQuestCode, newQuestInfo.QuestSubInfo));
}
