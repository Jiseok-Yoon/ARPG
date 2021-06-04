#pragma once

#include "ARPG.h"
#include "Blueprint/UserWidget.h"

#include "Struct/NpcInfo/NpcInfo.h"
#include "Struct/NpcDialogInfo/NpcDialogInfo.h"

#include "NpcDialog.generated.h"

DECLARE_MULTICAST_DELEGATE(FNpcDialogEventSignature)

UCLASS()
class ARPG_API UNpcDialog final : 
	public UUserWidget
{
	GENERATED_BODY()

private :
	TSubclassOf<class UClosableWnd> BP_ClosableWnd;
	TSubclassOf<class UNpcShopWnd> BP_NpcShopWnd;

	class UDataTable* DT_ShopInfo;

public :
	// Npc Dialog Widget 이 닫힐 경우 호출될 대리자
	FNpcDialogEventSignature OnDlgClosed;

private :
	// Npc 상점 창 객체를 나타냅니다.
	class UNpcShopWnd* NpcShopWnd;

	FNpcInfo* NpcInfo;

private :
	// 표시되는 대화 정보를 나타냅니다.
	FNpcDialogInfo DialogInfos;

	// 현재 몇 번째 대화를 표시하는 지 나타냅니다.
	UPROPERTY()
	int32 CurrentDialogIndex;

	// 마지막 대화임을 나타냅니다.
	bool bIsLastDialog;


private :
	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Exit;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Next;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_GoToFirst;
	
	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Shop;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_NpcName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Dialog;

public :
	UNpcDialog(const FObjectInitializer& ObjectInitializer);

protected :
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public :
	// 다이얼로그 객체 초기화
	void InitializeNpcDialog(FNpcInfo* npcInfo);

private :
	// 다음 대화 버튼의 가시성을 설정합니다.
	void SetNextDialogButtonVisibility(bool bVisible);

	// 지정한 순서의 대화를 표시합니다.
	void ShowDialog(int32 newDialogIndex);


#pragma region ButtonEvent

private :
	// 다이얼로그를 초기화합니다.
	UFUNCTION()
	void InitializeDialog();

	UFUNCTION()
	void CreateShopWnd();

	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void OnNextDialogButtonClicked();

#pragma endregion
	
};
