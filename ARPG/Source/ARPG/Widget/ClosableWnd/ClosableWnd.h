#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/InputModeType.h"
#include "ClosableWnd.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FWndEventSignature, class UClosableWnd*)

UCLASS()
class ARPG_API UClosableWnd : public UUserWidget
{
	GENERATED_BODY()

public :
	UPROPERTY()
	class UWidgetControllerWidget* WidgetController;

	// 창이 닫힐 때 호출되는 대리자
	FWndEventSignature OnWndClosedEvent;

	// 부모 창을 나타냅니다.
	/// - 부모 창이 nullptr 가 아니라면 부모 창이 닫힐 때 해당 창도 닫히게 됩니다.
	UPROPERTY()
	UClosableWnd* ParentWnd;

	// 자식 창 객체들을 저장합니다.
	TArray<UClosableWnd*> ChildWnds;

	// 닫힘 처리된 창임을 나타냅니다.
	UPROPERTY()
	bool bBeClose;



private :
	UPROPERTY(meta = (BindWidget))
	class UDraggableWndTitle * BP_DraggableWndTitle;

protected :
	UPROPERTY()
	class UButton * CloseButton;

	// 창 크기를 나타냅니다.
	FVector2D WndSize;

protected :
	virtual void NativeConstruct() override;

public :
	// 닫기 버튼을 설정합니다.
	void SetCloseButton(class UButton* closeButtont);

	// 자식 창을 생성합니다.
	/// - closableWndClass : 생성할 창 클래스를 전달합니다.
	/// - bUsePrevPosition : 이전 창 위치 사용 여부를 전달합니다.
	/// - changeInputMode : 변경시킬 입력 모드를 전달합니다.
	/// - bShowMouseCursor : 커서 표시 여부를 전달합니다.
	/// - return : 생성된 창이 반환됩니다.
	UClosableWnd* CreateChildClosableWnd(
		TSubclassOf<UClosableWnd> closableWndClass,
		bool bUsePrevPosition = false,
		EInputModeType changeInputMode = EInputModeType::IM_Default,
		bool bShowMouseCursor = true);

	// 이 창을 닫습니다.
	UFUNCTION()
	virtual void CloseThisWnd();

	// 모든 자식 창을 닫습니다.
	void CloseAllChildWnd();

	// 부모 창에서 떼어냅니다.
	void DetachFromParent();

public :
	// 이 창의 CanvasPanelSlot 을 얻습니다.
	class UCanvasPanelSlot* GetCanvasPanelSlot() const;

	// 창의 크기를 반환합니다.
	FORCEINLINE FVector2D GetWndSize() const
	{ return WndSize; }

	// 창의 제목을 설정합니다.
	void SetTitleText(FText newTitleText);
	FORCEINLINE void SetTitleText(FString newTitleText)
	{ SetTitleText(FText::FromString(newTitleText)); }
	

	
};
