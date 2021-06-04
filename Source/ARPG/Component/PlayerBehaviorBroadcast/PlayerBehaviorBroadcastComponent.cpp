#include "PlayerBehaviorBroadcastComponent.h"

UPlayerBehaviorBroadcastComponent::UPlayerBehaviorBroadcastComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerBehaviorBroadcastComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerBehaviorEvents.Add(BH_None, FPlayerBehaviorEvent());
	PlayerBehaviorEvents.Add(BH_GetItem, FPlayerBehaviorEvent());
	PlayerBehaviorEvents.Add(BH_StartQuest, FPlayerBehaviorEvent());
	PlayerBehaviorEvents.Add(BH_FinishQuest, FPlayerBehaviorEvent());
}

void UPlayerBehaviorBroadcastComponent::TickComponent(
	float DeltaTime, 
	ELevelTick TickType, 
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	BehaviorProcedure();
}

void UPlayerBehaviorBroadcastComponent::BehaviorProcedure()
{
	// 발생한 행동이 존재하지 않는다면 실행하지 않습니다.
	if (PlayerBehaviors.IsEmpty()) return;

	// 추가된 행동을 얻습니다.
	FPlayerBehaviorData newBehavior;
	PlayerBehaviors.Dequeue(newBehavior);

	// 행동 타입에 따라 이벤트를 실행시킵니다.
	PlayerBehaviorEvents[newBehavior.Behavior].Broadcast(newBehavior);
}

void UPlayerBehaviorBroadcastComponent::AddBehavior(FPlayerBehaviorData newBehaviorData)
{
	PlayerBehaviors.Enqueue(newBehaviorData);
}

