#include "QuestInfo.h"

#include "Single/PlayerManager/PlayerManager.h"

bool FQuestInfo::IsProgress(UPlayerManager* playerManager, FName questCode)
{
	FPlayerCharacterInfo* playerCharacterInfo = playerManager->GetPlayerInfo();

	for (auto info : playerCharacterInfo->ProgressQuestInfos)
		if (info.QuestCode == questCode) return true;

	return false;
}

bool FQuestInfo::IsOrderable(UPlayerManager* playerManager, FName questCode)
{
	FPlayerCharacterInfo* playerCharacterInfo = playerManager->GetPlayerInfo();

	// 수주 가능한 캐릭터 클래스인지 확인합니다.
	if (QuestAvailableClass != ECharacterClassType::CLS_ALL &&
		QuestAvailableClass != playerCharacterInfo->CharacterClass)
		return false;

	// 레벨 확인
	if (QuestAvailableLevel > playerCharacterInfo->CharacterLevel)
		return false;

	// 진행중인 퀘스트인지 확인
	for (auto questInfo : playerCharacterInfo->ProgressQuestInfos)
		// 진행중인 퀘스트라면 수주 불가능
		if (questCode == questInfo.QuestCode) return false;

	// 선행 퀘스트 완료 확인
	for (auto questCode : PrecedentQuestList)
	{
		// 선행 퀘스트 미완료를 나타냅니다.
		bool incompleted = true;

		for (auto completedQuestCode : playerCharacterInfo->CompletedQuestCodes)
		{
			if (questCode == completedQuestCode)
			{
				// 선행 퀘스트 완료함
				incompleted = false;
				break;
			}
		}

		// 선행 퀘스트(questCode)를 완료하지 않았다면 수주 불가능
		if (incompleted) return false;
	}

	return true;
}
