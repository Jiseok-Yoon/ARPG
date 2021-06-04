#include "SkillControllerComponent.h"

#include "Single/GameInstance/RPGGameInst.h"

#include "Actor/Level/BaseLevelScriptActor.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"


USkillControllerComponent::USkillControllerComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SKILL_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_SkillInfo.DT_SkillInfo'"));
	if (DT_SKILL_INFO.Succeeded()) DT_SkillInfo = DT_SKILL_INFO.Object;

	PrimaryComponentTick.bCanEverTick = true;

	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	SkillCount = 0;
	bIsRequestable = true;
	CurrentSkillInfo = PrevSkillInfo = nullptr;

}

void USkillControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentLevel = Cast<ABaseLevelScriptActor>(GetWorld()->GetLevelScriptActor());
}

void USkillControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SkillProcedure();
}

void USkillControllerComponent::SkillProcedure()
{
	// 현재 실행중인 스킬이 존재한다면 return
	/// - 스킬은 순차적으로 하나씩 진행되도록 합니다.
	if (CurrentSkillInfo != nullptr) return;

	// 큐에 요소가 존재하지 않는다면 실행하지 않습니다.
	if (SkillQueue.IsEmpty()) return;

	// 다음에 실행시킬 스킬 정보를 얻습니다.
	FSkillInfo* skillInfo;
	SkillQueue.Dequeue(skillInfo);

	--SkillCount;

	// 스킬 상태 갱신
	UpdateUsedSkillInfo(skillInfo);

	// 스킬을 실행합니다.
	CastSkill(skillInfo);
}

void USkillControllerComponent::UpdateUsedSkillInfo(FSkillInfo* newSkillInfo)
{
	// 전에 사용한 스킬이 현재 사용될 스킬과 같은 스킬이 아니라면, 이전 스킬이 다음에 사용되었을 때 
	// 연계 시작 가능한 상태로 설정합니다.
	if (PrevSkillInfo != nullptr)
	{
		if (PrevSkillInfo->SkillCode != newSkillInfo->SkillCode)
		{
			UsedSkillInfo[PrevSkillInfo->SkillCode].SkillCombo = -1;
		}
	}

	// 이전에 스킬이 사용된 적이 있다면
	if (UsedSkillInfo.Contains(newSkillInfo->SkillCode))
	{
		// 콤보를 사용하는 스킬이라면
		if (newSkillInfo->GetMaxComboCount() != 0)
		{
			// 콤보 카운트를 증가시킵니다.
			UsedSkillInfo[newSkillInfo->SkillCode].AddCombo();

			// 콤보 카운트가 최대 콤보 카운트를 초과한다면 리셋시킵니다.
			if (UsedSkillInfo[newSkillInfo->SkillCode].SkillCombo == newSkillInfo->GetMaxComboCount())
			{
				UsedSkillInfo[newSkillInfo->SkillCode].ResetCombo();

				// 사용했던 스킬 범위 인덱스를 초기화합니다.
				UsedSkillInfo[PrevSkillInfo->SkillCode].ResetSkillRangeIndex();
			}


		}

	}
	// 스킬이 처음 사용된다면 새로운 데이터를 추가합니다.
	else 
	{
		FSkillProgressInfo newSkillProgressInfo = FSkillProgressInfo(
			newSkillInfo->SkillCode,
			0.0f, 0.0f,
			0);

		UsedSkillInfo.Add(newSkillInfo->SkillCode, newSkillProgressInfo);
	}
}

void USkillControllerComponent::CastSkill(FSkillInfo* skillInfo)
{
	// 새로운 스킬을 현재 사용 스킬로 설정합니다.
	CurrentSkillInfo = skillInfo;

	// 스킬 요청 불가능 상태로 설정합니다.
	bIsRequestable = false;






	// 애니메이션을 재생합니다.
	UAnimMontage* skillAnimMontage = Cast<UAnimMontage>(
		GetManager(FStreamableManager)->LoadSynchronous(CurrentSkillInfo->AnimMontagePath));

	// 이전에 사용한 스킬이 존재한다면
	if (PrevSkillInfo != nullptr)
	{
		// 같은 스킬을 연계 공격으로 사용했다면
		if ((PrevSkillInfo->SkillCode == CurrentSkillInfo->SkillCode) &&
			CurrentSkillInfo->LinkableSkillSectionNames.Num() != 0)
		{
			// 애님 몽타주 섹션 이름을 얻습니다.
			int32 currentComboCount = UsedSkillInfo[CurrentSkillInfo->SkillCode].SkillCombo;
			FName sectionName = CurrentSkillInfo->LinkableSkillSectionNames[currentComboCount];

			// 지정한 섹션의 애니메이션을 재생합니다.
			PlayerCharacter->PlayAnimMontage(skillAnimMontage, 1.0f, sectionName);
			return;
		}
		// 다른 스킬을 사용했다면
		else 
		{
			// 이전에 사용한 스킬을 다음에 사용되었을 때 연계 시작 가능한 상태로 설정합니다.
			UsedSkillInfo[PrevSkillInfo->SkillCode].SkillCombo = -1;
		}
	}

	PlayerCharacter->PlayAnimMontage(skillAnimMontage);
}

void USkillControllerComponent::MakeSkillRange(
	FSkillInfo* skillInfo,
	FVector tracingStart, 
	FVector tracingEnd, 
	float radius, 
	FName profileName)
{
	// 감지 무시 대상 액터들을 나타냅니다.
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(GetOwner());

	// 감지 결과를 담을 배열을 나타냅니다.
	TArray<FHitResult> hitResults;


	UKismetSystemLibrary::SphereTraceMultiByProfile(
		GetWorld(),
		tracingStart,
		tracingEnd,
		radius,
		profileName,
		true,
		actorsToIgnore,
		EDrawDebugTrace::Type::ForDuration,
		hitResults,
		true);
	
	for(FHitResult hitresult : hitResults) 
	{
		CurrentLevel->CreatePoolableParticleActor(
			Cast<UParticleSystem>(GetManager(FStreamableManager)->LoadSynchronous(skillInfo->HitParticle)),
			hitresult.ImpactPoint);

	}

}

void USkillControllerComponent::RequestSkill(FName skillCode)
{
	// 스킬을 요청할 수 없는 경우 실행하지 않습니다.
	if (!bIsRequestable) return;

	// 스킬이 3개 이상 등록됐다면 실행시키지 않습니다.
	if (SkillCount >= 3) return;

	// 요청한 스킬 정보를 얻습니다.
	FString contextString;
	FSkillInfo* requestSkillInfo = DT_SkillInfo->FindRow<FSkillInfo>(skillCode, contextString);

	// 요청한 스킬을 찾지 못했을 경우 요청 취소
	if (requestSkillInfo == nullptr) return;

	// 실행시킬 스킬을 담는 큐에 추가합니다.
	SkillQueue.Enqueue(requestSkillInfo);
	++SkillCount;
}

void USkillControllerComponent::SkillFinished()
{
	// 이전 스킬에 현재 스킬 정보를 저장합니다.
	PrevSkillInfo = CurrentSkillInfo;
	CurrentSkillInfo = nullptr;

	// 스킬 요청 가능 상태로 설정합니다.
	bIsRequestable = true;

	// 사용할 스킬이 존재하지 않는다면
	if (SkillCount == 0)
	{
		// 이전에 사용한 스킬을 다음에 사용되었을 때 연계 시작 가능한 상태로 설정합니다.
		UsedSkillInfo[PrevSkillInfo->SkillCode].SkillCombo = -1;

		// 사용했던 스킬 범위 인덱스를 초기화합니다.
		UsedSkillInfo[PrevSkillInfo->SkillCode].ResetSkillRangeIndex();
	}
}

void USkillControllerComponent::NextSkillRangeIndex()
{
	// 현재 실행중인 스킬이 존재하지 않는다면 실행하지 않습니다.
	if (CurrentSkillInfo == nullptr) return;

	// 스킬 범위 인덱스 변경
	++UsedSkillInfo[CurrentSkillInfo->SkillCode].CurrentSkillRangeIndex;

	// 만약 인덱스가 배열 범위를 초과한다면 마지막 요소를 사용하도록 합니다.
	if (UsedSkillInfo[CurrentSkillInfo->SkillCode].CurrentSkillRangeIndex ==
		CurrentSkillInfo->SkillRangeInfos.Num())
		--UsedSkillInfo[CurrentSkillInfo->SkillCode].CurrentSkillRangeIndex;
}

void USkillControllerComponent::MakeSkillRange()
{
	int32 currentSkillRangeIndex = UsedSkillInfo[CurrentSkillInfo->SkillCode].CurrentSkillRangeIndex;

	// 스킬 범위 정보를 얻습니다.
	FSkillRangeInfo skillRangeInfo = CurrentSkillInfo->SkillRangeInfos[currentSkillRangeIndex];
	TTuple<int32, float> calcFormulaRes = skillRangeInfo.GetSkillCalcFormulaResult();
	int32 rangeCount = calcFormulaRes.Get<0>();
	float value = calcFormulaRes.Get<1>();

	// Sphere Tracing 을 사용하지 않는다면 실행하지 않습니다.
	if (!skillRangeInfo.bUseSphereTrace) return;

	// 트레이싱 오프셋을 얻습니다.
	FVector tracingOffset =
		(GetOwner()->GetActorForwardVector() * skillRangeInfo.TraceStartOffset.X) +
		(GetOwner()->GetActorRightVector() * skillRangeInfo.TraceStartOffset.Y) +
		(GetOwner()->GetActorUpVector() * skillRangeInfo.TraceStartOffset.Z);

	// 트레이싱 시작 위치를 나타냅니다.
	FVector tracingStart = 
		GetOwner()->GetActorLocation() + tracingOffset;

	// 트레이싱 방향
	FVector tracingDirection = skillRangeInfo.GetTraceDirection(GetOwner());

	// 트레이싱 끝 위치를 나타냅니다.
	FVector tracingEnd = tracingStart + (tracingDirection * skillRangeInfo.TraceDistance);

	// 사용하는 스킬 정보를 나타냅니다.
	FSkillInfo* skillInfo = CurrentSkillInfo;

	for (int32 i = 0; i < rangeCount; ++i)
	{
		if (FMath::IsNearlyZero(skillRangeInfo.CreateDelay) || (i == 0))
			MakeSkillRange(skillInfo, tracingStart, tracingEnd, skillRangeInfo.TraceRadius, FName(TEXT("PlayerSkill")));
		else
		{
			FTimerHandle timerHandle;
			FTimerDelegate timerDelegate;
			timerDelegate.BindLambda([skillInfo, tracingStart, tracingEnd, skillRangeInfo, this]()
				{
					MakeSkillRange(skillInfo, tracingStart, tracingEnd, skillRangeInfo.TraceRadius, FName(TEXT("PlayerSkill")));
				});

			// 타이머 실행
			GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, skillRangeInfo.CreateDelay * i, false);

		}
	}
}
