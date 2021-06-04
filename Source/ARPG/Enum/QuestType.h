#pragma once

#include "CoreMinimal.h"
#include "QuestType.generated.h"

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	MainQuest		UMETA(DisplayName = 메인 퀘스트),
	SubQuest		UMETA(DisplayName = 서브 퀘스트)
};
