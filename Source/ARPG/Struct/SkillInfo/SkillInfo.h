#pragma once

#include "ARPG.h"
#include "Engine/DataTable.h"
#include "Struct/SkillRangeInfo/SkillRangeInfo.h"
#include "SkillInfo.generated.h"

USTRUCT(BlueprintType)
struct ARPG_API FSkillInfo : 
	public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// 스킬 코드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillCode;

	// 스킬 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillName;

	// 스킬 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillDescription;

	// 아이콘 이미지 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath SkillIconPath;

	// 같은 스킬을 연계할 경우 콤보별 섹션 이름을 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> LinkableSkillSectionNames;

	// AnimMontage 애셋 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath AnimMontagePath;

	// 스킬 범위를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkillRangeInfo> SkillRangeInfos;

	// 타격 시 플레이될 파티클 애셋 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath HitParticle;




	// 같은 스킬을 연계할 때 쌓을 수 있는 최대 콤보 카운트를 반환합니다.
	FORCEINLINE int32 GetMaxComboCount() const
	{ return LinkableSkillSectionNames.Num(); }
};
