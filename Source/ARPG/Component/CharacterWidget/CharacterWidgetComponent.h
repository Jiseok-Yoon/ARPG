#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CharacterWidgetComponent.generated.h"

UCLASS()
class ARPG_API UCharacterWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public :
	// 위젯을 초기화합니다.
	void InitializeCharacterWidget(
		TSubclassOf<class UCharacterWidget> widgetClass,
		FVector2D drawSize);

	// 위젯의 높이를 설정합니다.
	void SetWidgetHeight(float height);

public :
	// 현재 띄워진 CharacterWidget 을 반환합니다.
	class UCharacterWidget* GetCharacterWidget() const;


	
};
