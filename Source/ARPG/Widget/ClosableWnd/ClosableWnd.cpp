#include "ClosableWnd.h"

#include "Components/CanvasPanelSlot.h"

#include "Widget/ClosableWnd/DraggableWndTitle/DraggableWndTitle.h"
#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"

#include "Components/Button.h"

void UClosableWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// 창 크기를 설정합니다.
	WndSize = FVector2D(500.0f, 600.0f);

	BP_DraggableWndTitle->SetTargetWidget(this);

	// 드래깅이 시작될 때 이 창을 최상단으로 배치하도록 합니다.
	BP_DraggableWndTitle->OnDraggingStarted.AddLambda(
		[this]() { WidgetController->SetHighestPriorityWnd(this); });

	SetCloseButton(BP_DraggableWndTitle->GetCloseButton());

}

void UClosableWnd::SetCloseButton(UButton* closeButtont)
{
	(CloseButton = closeButtont)->OnClicked.AddDynamic(this, &UClosableWnd::CloseThisWnd);
}

UClosableWnd* UClosableWnd::CreateChildClosableWnd(TSubclassOf<UClosableWnd> closableWndClass, 
	bool bUsePrevPosition, EInputModeType changeInputMode, bool bShowMouseCursor)
{
	// 자식 창을 생성합니다.
	auto childWnd = WidgetController->CreateWnd(
		closableWndClass, bUsePrevPosition,
		changeInputMode, bShowMouseCursor);

	// 생성된 자식 창의 부모 창을 자신으로 설정합니다.
	childWnd->ParentWnd = this;

	// 배열에 추가합니다.
	ChildWnds.Add(childWnd);

	// 생성된 자식 창을 반환합니다.
	return childWnd;
}

void UClosableWnd::CloseThisWnd()
{
	if (bBeClose) return;

	WidgetController->CloseWnd(false, this);
}

void UClosableWnd::CloseAllChildWnd()
{
	// 자식 창이 존재한다면
	if (ChildWnds.Num() != 0)
	{
		// 모든 자식 창을 제거합니다.
		for (int32 i = 0; i < ChildWnds.Num(); ++i)
		{ 
			auto childWnd = ChildWnds[i];

			if (IsValid(childWnd))
			{
				// 곧 닫힐 창이라면 실행하지 않습니다.
				if (childWnd->bBeClose) continue;

				WidgetController->CloseWnd(
					/*bAllClose = */ false,
					/*closableWndInstanceToClose = */ childWnd);
			}
		}

		ChildWnds.Empty();
	}
}

void UClosableWnd::DetachFromParent()
{
	// 부모 창이 존재한다면
	if (IsValid(ParentWnd))
	{
		ParentWnd->ChildWnds.Remove(this);
	}
}

UCanvasPanelSlot* UClosableWnd::GetCanvasPanelSlot() const
{
	return Cast<UCanvasPanelSlot>(Slot);
}

void UClosableWnd::SetTitleText(FText newTitleText)
{
	BP_DraggableWndTitle->SetTitleText(newTitleText);
}
