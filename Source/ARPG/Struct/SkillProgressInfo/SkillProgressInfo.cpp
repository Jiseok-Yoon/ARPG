#include "SkillProgressInfo.h"

FSkillProgressInfo::FSkillProgressInfo()
{
	ProgressSkillCode = FName();
	SkillCoolTime = SkillCastTime = 0.0f;
	SkillCombo = 0;
	CurrentSkillRangeIndex = 0;
}

FSkillProgressInfo::FSkillProgressInfo(
	FName progressSkillCode, 
	float skillCoolTime, 
	float skillCastTime, 
	int32 skillCombo,
	int32 currentSkillRangeIndex)
{
	ProgressSkillCode = progressSkillCode;
	SkillCoolTime = skillCoolTime;
	SkillCastTime = skillCastTime;
	SkillCombo = skillCombo;
	CurrentSkillRangeIndex = currentSkillRangeIndex;
}

