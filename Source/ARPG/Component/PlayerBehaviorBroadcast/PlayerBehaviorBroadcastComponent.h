#pragma once

#include "ARPG.h"
#include "Components/ActorComponent.h"

#include "Struct/PlayerBehaviorData/PlayerBehaviorData.h"

#include "PlayerBehaviorBroadcastComponent.generated.h"


DECLARE_EVENT_OneParam(UPlayerBehaviorBroadcastComponent, FPlayerBehaviorEvent, FPlayerBehaviorData&)
// 이벤트
/// - 해당 클래스 내에서만 호출할 수 있는 대리자입니다.
/// - 기본적으로 MULTICAST 입니다.
/// - 형식 선언 방식 DECLARE_EVENT(이벤트 사용 클래스명, 이벤트 형식, 매개 변수 타입들...)



UCLASS()
class ARPG_API UPlayerBehaviorBroadcastComponent : public UActorComponent
{
	GENERATED_BODY()

private :
	// 특정한 행동을 했을 때 호출되는 대리자들을 나타냅니다.
	TMap<EPlayerBehavior::Type, FPlayerBehaviorEvent> PlayerBehaviorEvents;

	// 행동 데이터가 저장됩니다.
	TQueue<FPlayerBehaviorData> PlayerBehaviors;

public:	
	UPlayerBehaviorBroadcastComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private :
	// 발생한 행동을 처리합니다.
	void BehaviorProcedure();

public :
	// 행동을 추가합니다.
	void AddBehavior(FPlayerBehaviorData newBehaviorData);




};
