#pragma once

#include "ARPG.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

UCLASS()
class ARPG_API AEnemyController : 
	public AAIController
{
	GENERATED_BODY()

private :
	// 자극 감지 객체에서 사용될 시각 자극 객체를 나타냅니다.
	class UAISenseConfig_Sight* AISightConfig;

	// 추적 목표 액터를 나타냅니다.
	AActor* TrackingTargetActor;


public :
	AEnemyController();

	// 추적 위치를 갱신합니다.
	/// - TrackingTargetActor 가 nullptr 인 경우 false 를 반환합니다.
	bool UpdateTrackingPosition();

private :
	// 시각 센스에 감지된 액터의 상태가 새롭게 갱신될 경우 호출될 메서드입니다.
	UFUNCTION()
	void OnSightUpdated(AActor* Actor, FAIStimulus Stimulus);
	/// - Actor : 상태가 갱신된 액터가 전달됩니다.
	/// - Stimulus : 상태 갱신 결과에 대한 세부적인 정보들을 나타냅니다.

	
};
