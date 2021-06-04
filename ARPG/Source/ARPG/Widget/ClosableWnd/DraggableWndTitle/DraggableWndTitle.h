#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DraggableWndTitle.generated.h"

DECLARE_MULTICAST_DELEGATE(FDraggingEventSignature)

UCLASS()
class ARPG_API UDraggableWndTitle final : 
	public UUserWidget
{
	GENERATED_BODY()

public :
	// 드래깅이 시작될 때 호출되는 대리자
	FDraggingEventSignature OnDraggingStarted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title Text")
	FText TitleText;

private :
	// 드래깅 중임을 나타냅니다.
	UPROPERTY()
	bool bIsDragging;

	// 드래깅 시킬 위젯을 나타냅니다.
	UPROPERTY()
	UUserWidget* TargetWidget;

private :
	// 드래깅을 체크할 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Draggable;
	
	// 창 닫기 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Close;

	// 창 타이틀 텍스트
	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Title;

	// 이전 입력 위치를 저장할 변수
	FVector2D PrevInputPosition;

	// 위젯의 이전 위치를 저장할 변수
	FVector2D PrevWidgetPosition;

protected :
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


private :
	// 위젯 드래그를 수행합니다.
	void DragWidget();

private :
	UFUNCTION()
	void OnWidgetDragStarted();

	UFUNCTION()
	void OnWidgetDragFinished();

public :
	void SetTitleText(FText newTitleText);

	FORCEINLINE void SetTargetWidget(UUserWidget* targetWidget)
	{ TargetWidget = targetWidget; }

	FORCEINLINE class UButton* GetCloseButton() const
	{ return Button_Close; }
	
};
