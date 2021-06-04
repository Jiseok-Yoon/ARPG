#include "ProgressQuestInfo.h"

FProgressQuestInfo::FProgressQuestInfo()
{
	QuestCode = FName();
}

FProgressQuestInfo::FProgressQuestInfo(FName questCode, TArray<FQuestSubInfo>& subInfos)
{
	QuestCode = questCode;
	SubInfos = subInfos;
}