#pragma once

#include "ARPG.h"
#include "Widget/ClosableWnd/ClosableWnd.h"
#include "NpcQuestListWnd.generated.h"

UCLASS()
class ARPG_API UNpcQuestListWnd : public UClosableWnd
{
	GENERATED_BODY()

private :
	TSubclassOf<class UNpcQuestListElem> BP_NpcQuestListElem;

private :
	TMap<FName, struct FQuestInfo*> OrderableQuests;
	TMap<FName, struct FQuestInfo*> QuestInProgress;

	TArray<class UNpcQuestListElem*> OrderableQuestElem;
	TArray<class UNpcQuestListElem*> ProgressQuestElem;

public :
	class UNpcDialog* NpcDialog;

private :
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_QuestList;


public :
	UNpcQuestListWnd(const FObjectInitializer& objInitializer);

protected :
	virtual void NativeConstruct() override;
	
public :
	FORCEINLINE void UpdateQuestInfo(
		TMap<FName, struct FQuestInfo*>& orderableQuests, 
		TMap<FName, struct FQuestInfo*>& progressQuests)
	{
		OrderableQuests = orderableQuests;
		QuestInProgress = progressQuests;
	}

	// 퀘스트 목록을 갱신합니다.
	void UpdateQuestList();

};
