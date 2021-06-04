#pragma once

#include "ARPG.h"

#include "Components/ActorComponent.h"

#include "Struct/SkillInfo/SkillInfo.h"
#include "Struct/SkillProgressInfo/SkillProgressInfo.h"

#include "SkillControllerComponent.generated.h"


UCLASS()
class ARPG_API USkillControllerComponent : public UActorComponent
{
	GENERATED_BODY()

private :
	class UDataTable* DT_SkillInfo;

private :
	class ABaseLevelScriptActor* CurrentLevel;

	// 플레이어 캐릭터를 나타냅니다.
	class APlayerCharacter* PlayerCharacter;

	// 현재 실행중인 스킬 정보를 나타냅니다.
	FSkillInfo* CurrentSkillInfo;

	// 바로 이전에 실행시킨 스킬 정보를 나타냅니다.
	FSkillInfo* PrevSkillInfo;

	// 실행했었던 스킬 정보를 담아둘 맵
	/// - 콤보와 쿨타임을 계산하기 위해 사용됩니다.
	TMap<FName, FSkillProgressInfo> UsedSkillInfo;

	// 실행시킬 스킬들을 담아둘 큐
	TQueue<FSkillInfo*> SkillQueue;

	// 입력된 스킬 카운트를 나타냅니다.
	int32 SkillCount;

	// 스킬을 요청할 수 있음을 나타냅니다.
	/// - 이 값이 false 라면 스킬 실행을 요청할 수 없습니다.
	bool bIsRequestable;

public:	
	USkillControllerComponent();

public:	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private :
	// 스킬을 순서대로 처리합니다.
	void SkillProcedure();
	
	// 사용된 스킬 상태를 갱신합니다.
	void UpdateUsedSkillInfo(FSkillInfo* newSkillInfo);

	// 스킬을 시전합니다.
	void CastSkill(FSkillInfo* skillInfo);

	void MakeSkillRange(
		FSkillInfo * skillInfo,
		FVector tracingStart, 
		FVector tracingEnd, 
		float radius, 
		FName profileName);

public :
	// 스킬 실행을 요청합니다.
	/// - skillCode : 요청시킬 스킬 코드를 전달합니다.
	void RequestSkill(FName skillCode);

public :
	// 스킬 요청 가능 상태를 설정합니다.
	FORCEINLINE void SetSkillRequestable(bool bRequestable)
	{ bIsRequestable = bRequestable; }

	// 스킬이 끝났음을 알립니다.
	void SkillFinished();

	// 스킬 범위 인덱스를 다음 인덱스로 변경합니다.
	void NextSkillRangeIndex();

	// 스킬 범위를 생성합니다.
	void MakeSkillRange();
		
};
