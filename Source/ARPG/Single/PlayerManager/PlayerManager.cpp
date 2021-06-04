#include "PlayerManager.h"

#include "Actor/Controller/PlayerController/BasePlayerController.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

#include "Struct/CharacterClassInfo/CharacterClassInfo.h"
#include "Struct/EquipItemInfo/EquipItemInfo.h"

UPlayerManager::UPlayerManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CHARACTER_CLASS_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_CharacterClassInfo.DT_CharacterClassInfo'"));
	if (DT_CHARACTER_CLASS_INFO.Succeeded()) DT_CharacterClassInfo = DT_CHARACTER_CLASS_INFO.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_EQUIP_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EquipItemInfo.DT_EquipItemInfo'"));
	if (DT_EQUIP_ITEM_INFO.Succeeded()) DT_EquipItemInfo = DT_EQUIP_ITEM_INFO.Object;

}

void UPlayerManager::RegisterPlayer(ABasePlayerController* newPlayerController,
	APlayerCharacter* newPlayerCharacter)
{
	PlayerController = newPlayerController;
	PlayerCharacter = newPlayerCharacter;
}

void UPlayerManager::InitManagerClass()
{
	PlayerInventory = NewObject<UPlayerInventory>(this, UPlayerInventory::StaticClass(),
		NAME_None, EObjectFlags::RF_MarkAsRootSet);
}

void UPlayerManager::ShutdownManagerClass()
{
	if (!PlayerInventory->IsValidLowLevel()) return;
	PlayerInventory->ConditionalBeginDestroy();
}

void UPlayerManager::InitializePlayerCharacter()
{
	// 캐릭터 정보
	FPlayerCharacterInfo* playerCharacterInfo = GetPlayerInfo();

	// 클래스 정보를 얻습니다.
	FName classKey = FName(*FString::FromInt(
		static_cast<uint32>(playerCharacterInfo->CharacterClass)));

	FString contextString;
	FCharacterClassInfo * newClassInfo = DT_CharacterClassInfo->FindRow<FCharacterClassInfo>(classKey, contextString);

	// 기본 장비 초기화
	playerCharacterInfo->InitializeDefaultEquipmentItems(
		newClassInfo->DefaultEquipItemCodes, DT_EquipItemInfo);

	// 장착된 모든 파츠 Mesh 를 비웁니다.
	for (TPair<EPartsType, USkeletalMeshComponent*> partsInfo : GetPlayerCharacter()->GetParts())
	{
		if (partsInfo.Key == EPartsType::PT_LGlove ||
			partsInfo.Key == EPartsType::PT_RGlove)
		{
			playerCharacterInfo->PartsInfos[EPartsType::PT_Glove].Clear();
		}
		else playerCharacterInfo->PartsInfos[partsInfo.Key].Clear();
		partsInfo.Value->SetSkeletalMesh(nullptr);
	}

	GetPlayerInventory()->UpdateCharacterVisual();

	GetPlayerCharacter()->InitializeMeshs();

}
