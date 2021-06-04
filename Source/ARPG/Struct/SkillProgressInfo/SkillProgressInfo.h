#pragma once

#include "CoreMinimal.h"
#include "SkillProgressInfo.generated.h"


USTRUCT()
struct ARPG_API FSkillProgressInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// 실행한 스킬 코드
	FName ProgressSkillCode;

	// 스킬 쿨타임
	float SkillCoolTime;

	// 스킬 시전 시간
	float SkillCastTime;

	// 스킬 콤보
	int32 SkillCombo;

	// 현재 스킬 범위 인덱스
	int CurrentSkillRangeIndex;

public :
	FSkillProgressInfo();
	FSkillProgressInfo(
		FName progressSkillCode, 
		float skillCoolTime, 
		float skillCastTime, 
		int32 skillCombo = 0, 
		int32 currentSkillRangeIndex = 0);

	// 콤보 카운트를 증가시킵니다.
	FORCEINLINE void AddCombo()
	{ ++SkillCombo; }

	FORCEINLINE void ResetCombo()
	{ SkillCombo = 0; }

	// 스킬 범위 인덱스를 초기화합니다.
	FORCEINLINE void ResetSkillRangeIndex()
	{ CurrentSkillRangeIndex = 0; }

};
