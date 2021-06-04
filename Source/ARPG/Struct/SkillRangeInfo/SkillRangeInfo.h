#pragma once

#include "ARPG.h"
#include "Enum/SkillRangeDirection.h"
#include "SkillRangeInfo.generated.h"

USTRUCT(BlueprintType)
struct ARPG_API FSkillRangeInfo
{
	GENERATED_USTRUCT_BODY()


public:
	// Sphere Trace 사용 여부를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseSphereTrace;

	// 캐릭터가 기준이 되는 트레이싱 시작 위치 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TraceStartOffset;

	// 트레이싱 방향
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillRangeDirection TraceDirection;

	// Sphere Trace 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceDistance;

	// Sphere Trace 구 반지름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceRadius;

	// 스킬 대미지 계산식을 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DamageCalcFormula;
	
	// 여러 영역 생성 시 영역 생성 딜레이를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CreateDelay;


public:
	FSkillRangeInfo();

	/// basedActor 를 기준으로 하는 TraceDirection 방향을 나타냅니다.
	FVector GetTraceDirection(AActor* basedActor);


	// 이 범위의 타격 횟수와, 대미지를 반환합니다.
	TTuple<int32, float> GetSkillCalcFormulaResult();

};
