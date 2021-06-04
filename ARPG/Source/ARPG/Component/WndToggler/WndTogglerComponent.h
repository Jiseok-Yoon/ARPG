#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"

#include "WndTogglerComponent.generated.h"


DECLARE_DELEGATE(FToggleEvent)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UWndTogglerComponent : public UActorComponent
{
	GENERATED_BODY()

private :
	// 토글 가능한 위젯을 토글했을 때 호출할 대리자들을 나타냅니다.
	TMap<TSubclassOf<class UObject>, FToggleEvent> ToggleEvents;

public :
	template<typename T>
	FORCEINLINE void RegisterToggleEvent(FToggleEvent toggleEvent)
	{ ToggleEvents.Add(T::StaticClass(), toggleEvent); }

	template<typename T>
	FORCEINLINE void ToggleWnd()
	{
		if (!ToggleEvents.Contains(T::StaticClass())) return;
		ToggleEvents[T::StaticClass()].ExecuteIfBound();
	}
};
