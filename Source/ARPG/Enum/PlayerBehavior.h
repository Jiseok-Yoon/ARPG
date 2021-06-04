#pragma once


// 행동 타입을 나타냅니다.
/// - 이 열거 형식에 요소를 추가하는 경우
///   UPlayerBehaviorBroadcastComponent 에서 추가된 행동과 일치하는 
///   PlayerBehaviorEvents 요소를 추가해야 합니다.
namespace EPlayerBehavior
{
	enum Type : uint8
	{
		// Default
		BH_None,

		// Item Event
		BH_GetItem,

		// Quest Event
		BH_StartQuest,
		BH_FinishQuest
	};
}