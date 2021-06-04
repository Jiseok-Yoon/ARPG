#include "DraggableWndTitle.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"

void UDraggableWndTitle::NativePreConstruct()
{
	Super::NativePreConstruct();
	Text_Title->SetText(TitleText);
}

void UDraggableWndTitle::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Draggable->OnPressed.AddDynamic(this, &UDraggableWndTitle::OnWidgetDragStarted);
	Button_Draggable->OnReleased.AddDynamic(this, &UDraggableWndTitle::OnWidgetDragFinished);
}

void UDraggableWndTitle::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	DragWidget();

}

void UDraggableWndTitle::DragWidget()
{
	// 드래깅 중이 아니라면 실행하지 않습니다.
	if (!bIsDragging) return;

	// 이동 시킬 위젯이 존재하지 않는다면 실행하지 않습니다.
	if (!IsValid(TargetWidget)) return;

	// 마우스 위치를 얻습니다.
	auto mousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	// 드래깅 시킬 위젯 슬롯을 얻습니다.
	auto targetSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);

	// 위젯의 위치를 설정합니다.
	targetSlot->SetPosition(
		PrevWidgetPosition + (mousePosition - PrevInputPosition));

	PrevInputPosition = mousePosition;
	PrevWidgetPosition = targetSlot->GetPosition();
}

void UDraggableWndTitle::OnWidgetDragStarted()
{
	// 이동시킬 위젯이 존재하지 않는다면 실행하지 않습니다.
	if (!IsValid(TargetWidget)) return;

	// 입력된 마우스 위치를 저장합니다.
	PrevInputPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	/// - UWidgetLayoutLibrary : 위젯 레이아웃에 관련된 유틸성 정적 메서드들을 제공하는 클래스
	/// - GetMousePositionOnViewport : 뷰포트 내의 마우스 위치를 반환합니다.

	// 위젯의 이전 위치를 저장합니다.
	PrevWidgetPosition = Cast<UCanvasPanelSlot>(TargetWidget->Slot)->GetPosition();

	if (OnDraggingStarted.IsBound())
		OnDraggingStarted.Broadcast();

	bIsDragging = true;
}

void UDraggableWndTitle::OnWidgetDragFinished()
{
	// 이동시킬 위젯이 존재하지 않는다면 실행하지 않습니다.
	if (!IsValid(TargetWidget)) return;

	bIsDragging = false;
}

void UDraggableWndTitle::SetTitleText(FText newTitleText)
{ Text_Title->SetText(newTitleText); }
