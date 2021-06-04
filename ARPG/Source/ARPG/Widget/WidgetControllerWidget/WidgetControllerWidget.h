#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/InputModeType.h"

#include "WidgetControllerWidget.generated.h"

UCLASS()
class ARPG_API UWidgetControllerWidget : public UUserWidget
{
	GENERATED_BODY()
	
private :
	TSubclassOf<class UMessageBoxWnd> BP_MessageBoxWnd;
	TSubclassOf<UUserWidget> BP_MessageBoxBackground;

private :
	class ABasePlayerController* PlayerController;

	// 해당 객체에 할당된 창 위젯들을 나타냅니다.
	TArray<class UClosableWnd*> AllocatedWnds;

	// 해당 객체에 할당된 창이 아닌 위젯 객체들을 나타냅니다.
	TArray<class UUserWidget* > AllocatedWidgets;

	// 닫힌 창들의 이전 위치를 저장합니다.
	TMap<FString, FVector2D> PrevClosedWndPositions;

private :
	// 창 객체들이 아닌 위젯들이 추가될 캔버스
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel * CanvasPanel_WidgetParent;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel * CanvasPanel_WndParent;

public :
	UWidgetControllerWidget(const FObjectInitializer& ObjectInitializer);

	void InitializeWidgetControllerWidget(class ABasePlayerController* basePlayerController);

private :
	// 입력 모드를 초기화합니다.
	/// - 열린 위젯이나, 창이 존재한다면 입력 모드가 초기화되지 않습니다.
	/// - bForceChange : 열린 위젯이나 창의 존재 여부를 무시하고 강제로 초기화 시킬 것인지에 대한 여부를 나타냅니다.
	void ResetInputMode(bool bForceChange = false);

	// 창의 위치를 저장합니다
	/// - closableWnd : 위치를 저장시킬 창 객체를 전달합니다.
	void SaveWndPosition(const class UClosableWnd* closableWnd);

public :
	// 메시지 박스를 생성합니다.
	/// - titleText : 메시지 박스의 타이틀 문자열을 전달합니다.
	/// - msg : 메시지 박스의 내용을 전달합니다.
	/// - bUseBackground : 메시지 박스 후면에 어두운 배경이 깔리도록 할 것인지를 결정합니다.
	/// - buttons : 추가될 버튼들을 전달합니다. EMessageBoxButton 형식을 OR 연산자와 함께 전달해야 합니다.
	FORCEINLINE class UMessageBoxWnd* CreateMessageBox(FString titleText, FString msg,
		bool bUseBackground = false, uint8 buttons = 0b00000001)
	{ return CreateMessageBox(FText::FromString(titleText), FText::FromString(msg), bUseBackground, buttons); }

	class UMessageBoxWnd* CreateMessageBox(FText titleText, FText msg,
		bool bUseBackground = false, uint8 buttons = 0b00000001);

	// 창을 생성합니다.
	/// - wndClass : 생성할 창 UClass 를 전달합니다.
	/// - bUsePrevPosition : 창의 이전 위치 사용 여부를 전달합니다.
	/// - changeInputMode : 창을 띄운 후 변경할 입력 모드를 전달합니다.
	/// - bShowMouseCursor : 창을 띄운 후 커서를 표시할 것인지를 전달합니다.
	/// - alignmentX : 정렬 기준점이 되는 X 위치를 전달합니다.
	/// - alignmentY : 정렬 기준점이 되는 Y 위치를 전달합니다.
	/// - anchorMinX : 배치 시 기준점이 되는 X 위치를 전달합니다.
	/// - anchorMinY : 배치 시 기준점이 되는 Y 위치를 전달합니다.
	/// - anchorMaxX : 배치 시 기준점이 되는 X 위치를 전달합니다.
	/// - anchorMaxY : 배치 시 기준점이 되는 Y 위치를 전달합니다.
	class UClosableWnd* CreateWnd(TSubclassOf<class UClosableWnd> wndClass,
		bool bUsePrevPosition = false,
		EInputModeType changeInputMode = EInputModeType::IM_UIOnly,
		bool bShowMouseCursor = true, 
		float alignmentX = 0.5f, float alignmentY = 0.5f,
		float anchorMinX = 0.5f, float anchorMinY = 0.5f,
		float anchorMaxX = 0.5f, float anchorMaxY = 0.5f);

	// 창을 닫습니다.
	/// - bAllClose : 모든 창을 닫도록 할 것인지에 대한 여부입니다.
	/// - closableWndInstanceToClose : 어떤 창을 닫을 것인지 지정합니다.
	///   만약 nullptr 가 전달된다면 마지막에 열었던 창이 닫힙니다.
	void CloseWnd(bool bAllClose = false, class UClosableWnd* closableWndInstanceToClose = nullptr);

	// 창이 아닌 위젯을 추가합니다.
	/// - 추가된 위젯은 CanvasPanel_WidgetParent 캔버스 하위로 추가되며,
	///   창 위젯들보다 렌더, 상호작용 우선순위가 낮습니다.
	/// - childWidgetInstance : 하위 위젯으로 추가할 위젯 객체를 전달합니다.
	/// - changeInputMode : 창을 띄운 후 변경할 입력 모드를 나타냅니다.
	/// - bShowMouseCursor : 창을 띄운 후 커서를 표시할 것인지를 나타냅니다.
	/// - width : 띄운 위젯의 너비를 나타냅니다.
	/// - height : 띄운 위젯의 높이를 나타냅니다.
	void AddChildWidget(class UUserWidget* childWidgetInstance,
		EInputModeType changeInputMode = EInputModeType::IM_UIOnly,
		bool bShowMouseCursor = true,
		float width = 1920.0f, float height = 1080.0f);

	// 추가시킨 하위 위젯을 닫습니다.
	void CloseChildWidget(class UUserWidget* childWidgetInstance);

	// 창을 최상단으로 설정합니다.
	/// - closableWndInstance : 최상단으로 배치할 창 객체를 전달합니다.
	void SetHighestPriorityWnd(class UClosableWnd* closableWndInstance);

public :
	// 그리드 패널의 요소들을 차례대로 정렬합니다.
	/// - gridPanelElem : 정렬시킬 그리드 패널 요소를 전달합니다.
	/// - maxColumnCount : 최대 행 개수를 전달합니다.
	/// - refCurrentColumCount : 현재 행 번호를 전달합니다.
	///   전달한 변수의 값은 메서드 내부에서 변경됩니다.
	static void SortGridPanelElem(class UUserWidget* gridPanelElem,
		int maxColumnCount, int& refCurrentColumCount);

};
