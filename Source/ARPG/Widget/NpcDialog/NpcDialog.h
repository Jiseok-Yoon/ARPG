#pragma once

#include "ARPG.h"
#include "Blueprint/UserWidget.h"

#include "Struct/NpcInfo/NpcInfo.h"
#include "Struct/NpcDialogInfo/NpcDialogInfo.h"

#include "NpcDialog.generated.h"

DECLARE_MULTICAST_DELEGATE(FNpcDialogEventSignature)

DECLARE_EVENT(UNpcDialog, FQuestButtonEvent)

UCLASS()
class ARPG_API UNpcDialog final : 
	public UUserWidget
{
	GENERATED_BODY()

private :
	TSubclassOf<class UNpcShopWnd> BP_NpcShopWnd;
	TSubclassOf<class UNpcQuestListWnd> BP_NpcQuestListWnd;

	class UDataTable* DT_ShopInfo;
	class UDataTable* DT_QuestInfo;

public :
	// Npc Dialog Widget 이 닫힐 경우 호출될 대리자
	FNpcDialogEventSignature OnDlgClosed;

	// 퀘스트 수락 버튼을 눌렀을 때 발생하는 이벤트
	FQuestButtonEvent OnAcceptButtonClickedOneshot;

private :
	// Npc 상점 창 객체를 나타냅니다.
	class UNpcShopWnd* NpcShopWnd;

	// Npc 퀘스트 창 객체를 나타냅니다.
	class UNpcQuestListWnd * NpcQuestListWnd;

	FNpcInfo* NpcInfo;

	// 진행중인 퀘스트 목록
	TMap<FName, struct FQuestInfo*> QuestInProgress;


	// 수주 가능한 퀘스트 목록
	TMap<FName, struct FQuestInfo*> OrderableQuests;

	// 표시되는 대화 정보를 나타냅니다.
	FNpcDialogInfo DialogInfos;

	// 퀘스트용 대화임을 나타냅니다.
	bool bIsQuestDialog;

	// 현재 몇 번째 대화를 표시하는 지 나타냅니다.
	UPROPERTY()
	int32 CurrentDialogIndex;

	// 마지막 대화임을 나타냅니다.
	bool bIsLastDialog;

	// 수락 가능한 퀘스트 코드를 나타냅니다.
	FName AcceptableQuestCode;


private :
	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Exit;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Next;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Accept;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Rejection;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_GoToFirst;
	
	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Shop;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Quest;

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

	// 대화 내용을 변경합니다.
	void ChangeDialogInfo(FNpcDialogInfo newDialogInfo, bool bIsQuestDlg = false);

private :
	// 퀘스트 정보 초기화
	void InitializeQuestInfos();

	// 버튼의 가시성을 설정합니다.
	void SetButtonVisibility(class UButton* button, bool bVisible);

	// 사용되지 않는 메뉴의 가시성을 설정합니다.
	void SetMenuButtonVisibility(bool bShopButtonVisible, bool bQuestButtonVisible);

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
	void OnQuestButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void OnNextDialogButtonClicked();

	UFUNCTION()
	void OnAcceptButtonClicked();

#pragma endregion

public :
	// 수락 가능한 퀘스트 코드를 설정합니다.
	FORCEINLINE void SetAcceptableQuestCode(FName acceptableQuestCode)
	{ AcceptableQuestCode = acceptableQuestCode; }

	
};
