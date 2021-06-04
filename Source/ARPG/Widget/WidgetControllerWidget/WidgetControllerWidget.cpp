#include "WidgetControllerWidget.h"

#include "Actor/Controller/PlayerController/BasePlayerController.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridSlot.h"

#include "Widget/ClosableWnd/MessageBoxWnd/MessageBoxWnd.h"
#include "Widget/ClosableWnd/ClosableWnd.h"


UWidgetControllerWidget::UWidgetControllerWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UMessageBoxWnd> BP_MESSAGE_BOX_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/MessageBoxWnd/BP_MessageBoxWnd.BP_MessageBoxWnd_C'"));
	if (BP_MESSAGE_BOX_WND.Succeeded()) BP_MessageBoxWnd = BP_MESSAGE_BOX_WND.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> BP_MESSAGE_BOX_BACKGROUND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/MessageBoxWnd/BP_MessageBoxBackground.BP_MessageBoxBackground_C'"));
	if (BP_MESSAGE_BOX_BACKGROUND.Succeeded()) BP_MessageBoxBackground = BP_MESSAGE_BOX_BACKGROUND.Class;
}

void UWidgetControllerWidget::InitializeWidgetControllerWidget(
	class ABasePlayerController* basePlayerController)
{
	PlayerController = basePlayerController;
}

void UWidgetControllerWidget::ResetInputMode(bool bForceChange)
{
	// 열린 창과 위젯이 존재하지 않는다면 입력 모드를 변경합니다.
	if (bForceChange ||
		((AllocatedWnds.Num() == 0) && (AllocatedWidgets.Num() == 0)))
	{
		// 입력 모드를 설정
		PlayerController->ChangeInputModeToDefault();

		// 커서 표시
		PlayerController->bShowMouseCursor = PlayerController->GetDefaultCursorVisibility();
	}
}

void UWidgetControllerWidget::SaveWndPosition(const UClosableWnd* closableWnd)
{
	FString wndClsName = closableWnd->GetClass()->GetName();
	FVector2D wndPosition = closableWnd->GetCanvasPanelSlot()->GetPosition();

	if (PrevClosedWndPositions.Contains(wndClsName))
		PrevClosedWndPositions[wndClsName] = wndPosition;

	else PrevClosedWndPositions.Add(wndClsName, wndPosition);
}

UMessageBoxWnd* UWidgetControllerWidget::CreateMessageBox(FText titleText, FText msg, bool bUseBackground, uint8 buttons)
{
	UUserWidget* msgBoxBackground = nullptr;

	if (bUseBackground)
	{
		msgBoxBackground = CreateWidget<UUserWidget>(this, BP_MessageBoxBackground);
		CanvasPanel_WndParent->AddChild(msgBoxBackground);

		UCanvasPanelSlot* messageBoxCanvasPanelSlot = Cast<UCanvasPanelSlot>(msgBoxBackground->Slot);
		messageBoxCanvasPanelSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		messageBoxCanvasPanelSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
	}

	UMessageBoxWnd* msgBox = Cast<UMessageBoxWnd>(
		CreateWnd(BP_MessageBoxWnd, false, EInputModeType::IM_UIOnly, true));

	msgBox->MsgBoxBackground = msgBoxBackground;

	msgBox->InitializeMessageBox(titleText, msg, buttons);

	return msgBox;
}

UClosableWnd* UWidgetControllerWidget::CreateWnd(
	TSubclassOf<class UClosableWnd> wndClass, 
	bool bUsePrevPosition, 
	EInputModeType changeInputMode, 
	bool bShowMouseCursor, 
	float alignmentX, float alignmentY, 
	float anchorMinX, float anchorMinY, 
	float anchorMaxX, float anchorMaxY)
{
	// 새로운 창 위젯을 생성합니다.
	UClosableWnd* newClosableWnd = CreateWidget<UClosableWnd>(this, wndClass);
	newClosableWnd->WidgetController = this;

	// 할당된 창 배열에 추가합니다.
	AllocatedWnds.Add(newClosableWnd);

	// CanvasPanel_WndParent 의 자식 위젯으로 추가합니다.
	CanvasPanel_WndParent->AddChild(newClosableWnd);

	// Anchor / Alignment 설정
	auto wndCanvasPanelSlot = newClosableWnd->GetCanvasPanelSlot();
	wndCanvasPanelSlot->SetAnchors(FAnchors(anchorMinX, anchorMinY, anchorMaxX, anchorMaxY));
	wndCanvasPanelSlot->SetAlignment(FVector2D(alignmentX, alignmentY));

	// 창 크기를 설정합니다.
	wndCanvasPanelSlot->SetSize(newClosableWnd->GetWndSize());

	// 이전 창 위치를 사용한다면
	if (bUsePrevPosition)
	{
		// 이전 창 위치가 기록되어 있는지 확인합니다.
		//if (PrevClosedWndPositions.Contains(newClosableWnd->GetClass()->GetName()))
		if (PrevClosedWndPositions.Contains(wndClass->GetName()))
		{
			// 창 위치를 이전 위치로 설정합니다.
			wndCanvasPanelSlot->SetPosition(PrevClosedWndPositions[wndClass->GetName()]);
		}
	}

	// 입력 모드와 커서 표시 여부를 변경합니다.
	if (changeInputMode != EInputModeType::IM_Default)
		PlayerController->SetInputModeFromNewInputModeType(changeInputMode);
	PlayerController->bShowMouseCursor = bShowMouseCursor;

	// 생성한 창 객체를 반환합니다.
	return newClosableWnd;
}

void UWidgetControllerWidget::CloseWnd(bool bAllClose, UClosableWnd* closableWndInstanceToClose)
{
	// 만약 열린 창이 존재하지 않는 경우 실행하지 않습니다.
	if (AllocatedWnds.Num() == 0) return;

	// 모든 창을 닫도록 하였다면
	if (bAllClose)
	{
		for (auto wnd : AllocatedWnds)
		{
			// 닫힘 처리된 창이라면 실행하지 않습니다.
			if (wnd->bBeClose) continue;
			wnd->bBeClose = true;

			// 창 닫힘 이벤트를 발생시킵니다.
			if (wnd->OnWndClosedEvent.IsBound())
				wnd->OnWndClosedEvent.Broadcast(wnd);

			// 모든 자식 창을 닫습니다.
			wnd->CloseAllChildWnd();

			// 창 위치를 저장합니다.
			SaveWndPosition(wnd);

			// 부모 창에서 떼어냅니다.
			wnd->DetachFromParent();

			// CanvasPanel_WndParent 위젯의 자식에서 제거합니다.
			CanvasPanel_WndParent->RemoveChild(wnd);
		}

		// 항당된 창 배열을 비웁니다.
		AllocatedWnds.Empty();
	}
	// 특정한 창만 닫도록 하였다면
	else
	{
		// 닫을 창이 지정되지 않았다면 마지막으로 열린 창을 닫을 창으로 지정합니다.
		closableWndInstanceToClose = (closableWndInstanceToClose != nullptr) ?
			closableWndInstanceToClose :
			AllocatedWnds[AllocatedWnds.Num() - 1];

		// 곧 닫힐 창이 아니라면
		if (!closableWndInstanceToClose->bBeClose)
		{
			// 닫힐 창으로 설정합니다.
			closableWndInstanceToClose->bBeClose = true;

			// 창 닫힘 이벤트를 발생시킵니다.
			if (closableWndInstanceToClose->OnWndClosedEvent.IsBound())
				closableWndInstanceToClose->OnWndClosedEvent.Broadcast(closableWndInstanceToClose);

			// 모든 자식 창을 제거합니다.
			closableWndInstanceToClose->CloseAllChildWnd();

			// 할당된 창 배열에서 제외합니다.
			AllocatedWnds.Remove(closableWndInstanceToClose);

			// 창 위치를 저장합니다.
			SaveWndPosition(closableWndInstanceToClose);

			// 부모 창에서 떼어냅니다.
			closableWndInstanceToClose->DetachFromParent();

			// CanvasPanel_WndParent 위젯의 자식에서 제거합니다.
			CanvasPanel_WndParent->RemoveChild(closableWndInstanceToClose);
		}

	}

	// 입력 모드 초기화
	ResetInputMode();
}

void UWidgetControllerWidget::AddChildWidget(UUserWidget* childWidgetInstance,
	EInputModeType changeInputMode, bool bShowMouseCursor,
	float width, float height)
{
	if (childWidgetInstance == nullptr)
	{
		UE_LOG(LogTemp, Error, 
			TEXT("WidgetControllerWidget.cpp :: %d LINE :: childWidgetInstance is nullptr"), __LINE__);
		return;
	}

	CanvasPanel_WidgetParent->AddChild(childWidgetInstance);
	/// - Widget->AddChild(childWidget) : childWidget 을 Widget 의 하위로 추가합니다.

	Cast<UCanvasPanelSlot>(childWidgetInstance->Slot)->SetSize(FVector2D(width, height));

	// 입력 모드와 커서 가시성을 설정합니다.
	PlayerController->SetInputModeFromNewInputModeType(changeInputMode);
	PlayerController->bShowMouseCursor = bShowMouseCursor;

	AllocatedWidgets.Add(childWidgetInstance);

}

void UWidgetControllerWidget::CloseChildWidget(UUserWidget* childWidgetInstance)
{
	if (childWidgetInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("WidgetControllerWidget.cpp :: %d LINE :: childWidgetInstance is nullptr"), __LINE__);
		return;
	}

	// 제거하려는 위젯을 AllocatedWidgets 에서 제거합니다.
	AllocatedWidgets.Remove(childWidgetInstance);

	// 제거하려는 위젯을 화면에서 떼어냅니다.
	CanvasPanel_WidgetParent->RemoveChild(childWidgetInstance);
	/// - 부모 위젯에서 자식 위젯을 제거합니다.

	//childWidgetInstance->RemoveFromParent();
	/// - 자식 위젯에서 부모와 연결을 끊습니다.

	// 입력 모드를 기본 값으로 설정합니다.
	ResetInputMode();
}

void UWidgetControllerWidget::SetHighestPriorityWnd(UClosableWnd* closableWndInstance)
{
	// 우선순위를 변경시킬 위젯의 슬롯을 얻습니다.
	UCanvasPanelSlot* wndSlot = closableWndInstance->GetCanvasPanelSlot();

	// 최상단으로 설정하려는 위젯이 최상단에 배치되어 있지 않다면
	if (CanvasPanel_WndParent->GetChildIndex(closableWndInstance) !=
		/// - GetChildIndex(widget) : widget 의 계층구조 순서를 반환합니다.
		CanvasPanel_WndParent->GetSlots().Num() - 1)
		/// - GetSlots() : 추가된 위젯들의 슬롯 정보를 저장하는 배열을 반환합니다.
	{
		// 위치, Anchor, Alignment 를 저장합니다.
		FVector2D prevPosition = wndSlot->GetPosition();
		FAnchors prevAnchors = wndSlot->GetAnchors();
		FVector2D prevAlignment = wndSlot->GetAlignment();

		// 위젯을 재등록합니다.
		CanvasPanel_WndParent->AddChild(closableWndInstance);

		// Anchor, Alignment, 위치, 크기를 재설정합니다.
		wndSlot = closableWndInstance->GetCanvasPanelSlot();
		wndSlot->SetAnchors(prevAnchors);
		wndSlot->SetAlignment(prevAlignment);
		wndSlot->SetPosition(prevPosition);
		wndSlot->SetSize(closableWndInstance->GetWndSize());
	}
}

void UWidgetControllerWidget::SortGridPanelElem(
	UUserWidget* gridPanelElem, 
	int maxColumnCount, 
	int& refCurrentColumCount)
{
	UGridSlot* gridSlot = Cast<UGridSlot>(gridPanelElem->Slot);

	if (!IsValid(gridSlot)) return;

	// 그리드 행과 열을 설정합니다.
	gridSlot->SetColumn(refCurrentColumCount % maxColumnCount);
	gridSlot->SetRow(refCurrentColumCount / maxColumnCount);
	++refCurrentColumCount;
}
