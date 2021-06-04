#include "NpcQuestListElem.h"

#include "Widget/ClosableWnd/NpcQuestListWnd/NpcQuestListWnd.h"
#include "Widget/NpcDialog/NpcDialog.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UNpcQuestListElem::InitializeQuestListElem(
	UNpcQuestListWnd* npcQuestListWnd, 
	FName questCode, 
	FQuestInfo questInfo,
	bool bIsProgress)
{
	NpcQuestListWnd = npcQuestListWnd;
	QuestCode = questCode;
	QuestInfo = questInfo;
	this->bIsProgress = bIsProgress;

	Button_Quest->OnClicked.AddDynamic(this, &UNpcQuestListElem::OnQuestButtonClicked);


	// 퀘스트 타입에 따라 Text_QuestType 위젯의 문자열을 설정합니다.
	switch (QuestInfo.QuestType)
	{
		case EQuestType::MainQuest: Text_QuestType->SetText(FText::FromString(TEXT("[메인]")));	break;
		case EQuestType::SubQuest:	Text_QuestType->SetText(FText::FromString(TEXT("[서브]")));	break;
	}

	// Text_QuestTitle 위젯의 문자열을 설정합니다.
	Text_QuestTitle->SetText(QuestInfo.QuestTitleText);

	// 진행중 텍스트의 가시성을 설정합니다.
	Text_IsProgress->SetVisibility(this->bIsProgress ? 
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UNpcQuestListElem::OnQuestButtonClicked()
{
	// 진행중인 퀘스트라면
	if (bIsProgress)
	{
		// 대화 내용을 변경합니다.
		NpcQuestListWnd->NpcDialog->ChangeDialogInfo(
			/*newDialogInfo = */ QuestInfo.AfterQuestDialog,
			/*bIsQuestDlg   = */ false);

		// 퀘스트 창을 닫습니다.
		NpcQuestListWnd->CloseThisWnd();
	}
	// 새롭게 시작되는 퀘스트라면
	else
	{
		// 대화 내용을 변경합니다.
		NpcQuestListWnd->NpcDialog->ChangeDialogInfo(
			/*newDialogInfo = */ QuestInfo.BeforQuestDialog,
			/*bIsQuestDlg   = */ true);

		// 수락 가능한 퀘스트를 설정합니다.
		NpcQuestListWnd->NpcDialog->SetAcceptableQuestCode(QuestCode);

		// 퀘스트 창을 닫습니다.
		NpcQuestListWnd->CloseThisWnd();
	}



}
