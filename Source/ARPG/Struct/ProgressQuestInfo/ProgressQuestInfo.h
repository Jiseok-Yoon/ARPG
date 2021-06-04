#pragma once

#include "CoreMinimal.h"
#include "Struct/QuestSubInfo/QuestSubInfo.h"
#include "ProgressQuestInfo.generated.h"

USTRUCT()
struct ARPG_API FProgressQuestInfo
{
	GENERATED_USTRUCT_BODY()

public :
	UPROPERTY()
	FName QuestCode;

	UPROPERTY()
	TArray<FQuestSubInfo> SubInfos;

public:
	FProgressQuestInfo();
	FProgressQuestInfo(FName questCode, TArray<FQuestSubInfo>& subInfos);

};
