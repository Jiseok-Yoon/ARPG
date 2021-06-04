#include "CharacterWidgetComponent.h"

#include "Widget/CharacterWidget/CharacterWidget.h"

void UCharacterWidgetComponent::InitializeCharacterWidget(
	TSubclassOf<UCharacterWidget> widgetClass,
	FVector2D drawSize)
{
	// 위젯 클래스를 설정합니다.
	SetWidgetClass(widgetClass);

	// 크기를 설정합니다.
	SetDrawSize(drawSize);

	// 위젯이 표시될 공간을 설정합니다.
	SetWidgetSpace(EWidgetSpace::Screen);
}

void UCharacterWidgetComponent::SetWidgetHeight(float height)
{
	SetRelativeLocation(FVector::UpVector * height);
}

UCharacterWidget* UCharacterWidgetComponent::GetCharacterWidget() const
{
	return Cast<UCharacterWidget>(GetUserWidgetObject());
}
