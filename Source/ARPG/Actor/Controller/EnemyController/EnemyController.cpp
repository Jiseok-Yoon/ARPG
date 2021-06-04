#include "EnemyController.h"

#include "Enum/Team.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"



AEnemyController::AEnemyController()
{
	// AI Controller 의 자극 감지 컴포넌트를 설정합니다.
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI_PERCEPTION")));
	/// - UAIPerceptionComponent : AI 의 특정한 자극을 감지하는 컴포넌트를 나타냅니다.
	/// - 이렇게 생성한 자극 감지 컴포넌트를 GetPerceptionComponent() 를 이용하여 다시 얻을 수 있습니다.
	
	// AI 의 팀을 설정합니다.
	SetGenericTeamId(FGenericTeamId(ETeam::Enemy));

	// Sight Sense
	{
		// 시각 자극 객체 생성
		AISightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SIGHT_PERCEPTION"));

		// 시야 거리를 설정합니다.
		AISightConfig->SightRadius = 500.0f;

		// 시야 상실 거리를 설정합니다.
		AISightConfig->LoseSightRadius = 650.0f;

		// 한쪽 눈의 시야각을 설정합니다.
		AISightConfig->PeripheralVisionAngleDegrees = 180.0f;

		// 자극이 소멸되기까지 걸리는 시간을 설정합니다.
		AISightConfig->SetMaxAge(2.0f);

		// 적이 감지되도록 합니다.
		AISightConfig->DetectionByAffiliation.bDetectEnemies = true;

		// 같은 팀이 감지되지 않도록 합니다.
		AISightConfig->DetectionByAffiliation.bDetectFriendlies = false;

		// 중립 팀이 감지되지 않도록 합니다.
		AISightConfig->DetectionByAffiliation.bDetectNeutrals = false;

		// 해당 설정을 사용하는 시각 자극 객체를 적용시킵니다.
		GetPerceptionComponent()->ConfigureSense(*AISightConfig);

		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnSightUpdated);
	}

	TrackingTargetActor = nullptr;
}

bool AEnemyController::UpdateTrackingPosition()
{
	if (TrackingTargetActor == nullptr) return false;

	else
	{
		FVector lineTraceStartLoc = TrackingTargetActor->GetActorLocation();
		FVector lineTraceEndLoc = lineTraceStartLoc + (FVector::DownVector * 1000.0f);

		TArray<AActor*> hitActors;
		FHitResult hitResult;

		if (UKismetSystemLibrary::LineTraceSingleByProfile(
			GetWorld(),
			lineTraceStartLoc,
			lineTraceEndLoc,
			FName(TEXT("SpawnEnemy")),
			true,
			hitActors,
			EDrawDebugTrace::None,
			hitResult,
			true))
		{
			GetBlackboardComponent()->SetValueAsVector(
				TEXT("TargetPosition"), hitResult.Location);

			return true;
		}
		else return false;
	}

}

void AEnemyController::OnSightUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// 시야에 들어왔다면
	if (Stimulus.WasSuccessfullySensed())
	{
		TrackingTargetActor = Actor;
	}
	// 시야에서 사라졌다면
	else
	{
		TrackingTargetActor = nullptr;
	}

}

