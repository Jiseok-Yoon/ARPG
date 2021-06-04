#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Struct/QuestInfo/QuestInfo.h"

#include "NpcQuestListElem.generated.h"

UCLASS()
class ARPG_API UNpcQuestListElem : public UUserWidget
{
	GENERATED_BODY()

private :
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Quest;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_QuestType;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_QuestTitle;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_IsProgress;

private :
	// 해당 위젯을 소유하는 Npc 퀘스트 목록 창 위젯을 나타냅니다.
	class UNpcQuestListWnd* NpcQuestListWnd;

	// 퀘스트 코드를 나타냅니다.
	FName QuestCode;

	// 퀘스트 정보를 나타냅니다.
	FQuestInfo QuestInfo;

	// 진행중인 의뢰임을 나타냅니다.
	bool bIsProgress;


public :
	// 퀘스트 목록 요소 위젯을 초기화합니다.
	/// - npcQuestListWnd : 해당 위젯을 소유하는 Npc 퀘스트 목록 창 위젯을 전달합니다.
	/// - questCode : 퀘스트 코드를 전달합니다.
	/// - questInfo : 퀘스트 정보를 전달합니다.
	void InitializeQuestListElem(
		class UNpcQuestListWnd* npcQuestListWnd,
		FName questCode,
		FQuestInfo questInfo,
		bool bIsProgress);

private :
	UFUNCTION()
	void OnQuestButtonClicked();
	
};
