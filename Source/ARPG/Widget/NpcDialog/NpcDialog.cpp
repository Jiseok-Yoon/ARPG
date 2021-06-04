#include "NpcDialog.h"

#include "Actor/Controller/PlayerController/BasePlayerController.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"
#include "Widget/ClosableWnd/NpcShopWnd/NpcShopWnd.h"
#include "Widget/ClosableWnd/NpcQuestListWnd/NpcQuestListWnd.h"

#include "Struct/QuestInfo/QuestInfo.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

UNpcDialog::UNpcDialog(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UNpcShopWnd> BP_NPC_SHOP_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/NpcShopWnd/BP_NpcShopWnd.BP_NpcShopWnd_C'"));
	if (BP_NPC_SHOP_WND.Succeeded()) BP_NpcShopWnd = BP_NPC_SHOP_WND.Class;

	static ConstructorHelpers::FClassFinder<UNpcQuestListWnd> BP_NPC_QUEST_LIST_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/NpcQuestListWnd/BP_NpcQuestListWnd.BP_NpcQuestListWnd_C'"));
	if (BP_NPC_QUEST_LIST_WND.Succeeded()) BP_NpcQuestListWnd = BP_NPC_QUEST_LIST_WND.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SHOP_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ShopInfo.DT_ShopInfo'"));
	if (DT_SHOP_INFO.Succeeded()) DT_ShopInfo = DT_SHOP_INFO.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_QUEST_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_QuestInfo.DT_QuestInfo'"));
	if (DT_QUEST_INFO.Succeeded()) DT_QuestInfo = DT_QUEST_INFO.Object;

	NpcShopWnd = nullptr;
	NpcQuestListWnd = nullptr;
}

void UNpcDialog::NativeConstruct()
{
	Super::NativeConstruct();

	// 이 위젯이 포커싱을 지원하도록 합니다.
	bIsFocusable = true;
	SetKeyboardFocus();
	// 키 입력은 포커싱 된 상태에서 이루어져야 합니다.

	Button_Exit->OnClicked.AddDynamic(this, &UNpcDialog::OnExitButtonClicked);
	Button_Next->OnClicked.AddDynamic(this, &UNpcDialog::OnNextDialogButtonClicked);
	Button_Accept->OnClicked.AddDynamic(this, &UNpcDialog::OnAcceptButtonClicked);
	Button_Rejection->OnClicked.AddDynamic(this, &UNpcDialog::InitializeDialog);
	Button_Quest->OnClicked.AddDynamic(this, &UNpcDialog::OnQuestButtonClicked);
	Button_Shop->OnClicked.AddDynamic(this, &UNpcDialog::CreateShopWnd);
	Button_GoToFirst->OnClicked.AddDynamic(this, &UNpcDialog::InitializeDialog);
}

FReply UNpcDialog::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::R)
	{
		// 다음 대화 내용을 표시합니다.
		OnNextDialogButtonClicked();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UNpcDialog::InitializeNpcDialog(FNpcInfo* npcInfo)
{
	NpcInfo = npcInfo;

	if (NpcInfo->IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("NpcDialog.cpp :: %d LINE :: NpcInfo is Empty!"), __LINE__);
		return;
	}

	// Npc 이름 표시
	Text_NpcName->SetText(NpcInfo->NpcName);

	// 다이얼로그 초기화
	InitializeDialog();

	// 상점이나 퀘스트를 사용하지 않는 Npc 라면 버튼을 감춥니다.
	SetMenuButtonVisibility(NpcInfo->IsShopUsing(), NpcInfo->IsQuestUsing());
}

void UNpcDialog::InitializeQuestInfos()
{
	QuestInProgress.Empty();
	OrderableQuests.Empty();

	for (auto code : NpcInfo->QuestCodes)
	{
		FString contextString;
		FQuestInfo* questInfo = DT_QuestInfo->FindRow<FQuestInfo>(code, contextString);

		// 진행중인 퀘스트를 찾아 QuestInProgress 에 저장합니다.
		if (FQuestInfo::IsProgress(GetManager(UPlayerManager), code))
			QuestInProgress.Add(code, questInfo);
		
		// 수주 가능한 퀘스트를 찾아 OrderbleQuests 에 저장합니다.
		if (questInfo->IsOrderable(GetManager(UPlayerManager), code))
			OrderableQuests.Add(code, questInfo);
	}
}

void UNpcDialog::SetButtonVisibility(class UButton* button, bool bVisible)
{
	button->SetVisibility(
		bVisible ?
		ESlateVisibility::Visible :
		ESlateVisibility::Collapsed);
}

void UNpcDialog::SetMenuButtonVisibility(bool bShopButtonVisible, bool bQuestButtonVisible)
{
	SetButtonVisibility(Button_Shop, bShopButtonVisible);
	SetButtonVisibility(Button_Quest, bQuestButtonVisible);
}

void UNpcDialog::ShowDialog(int32 newDialogIndex)
{
	// 사용 가능한 대화 텍스트가 존재하지 않는다면
	if (DialogInfos.DialogText.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NpcDialog.cpp :: %d LINE :: Usable Dialog Count Is Zero!"), __LINE__);
		return;
	}

	// 표시하려는 대화 인덱스가 배열 범위를 초과한다면
	else if (DialogInfos.DialogText.Num() <= newDialogIndex)
	{
		UE_LOG(LogTemp, Error,
			TEXT("NpcDialog.cpp :: %d LINE :: [Warning] Out Of Range! newDialogIndex is Changed (%d -> %d)"),
			__LINE__,
			newDialogIndex,
			DialogInfos.DialogText.Num() - 1);

		// 표시하려는 대화 인덱스를 마지막 대화 인덱스로 설정합니다.
		newDialogIndex = (DialogInfos.DialogText.Num() - 1);
	}

	// 텍스트 설정
	Text_Dialog->SetText(DialogInfos.DialogText[newDialogIndex]);

	// 마지막 대화임을 확인합니다.
	bIsLastDialog = (DialogInfos.DialogText.Num() - 1) == newDialogIndex;

	// 마지막 대화라면
	if (bIsLastDialog)
	{
		// 다음 버튼 숨기기
		SetButtonVisibility(Button_Next, false);

		// 퀘스트용 대화라면
		if (bIsQuestDialog)
		{
			// 수락 / 거절 버튼 표시
			SetButtonVisibility(Button_Rejection, true);
			SetButtonVisibility(Button_Accept, true);
		}
	}
	else
	{
		// 다음 대화 버튼 표시
		SetButtonVisibility(Button_Next, true);
	}
}

void UNpcDialog::InitializeDialog()
{
	bIsLastDialog = false;
	bIsQuestDialog = false;
	AcceptableQuestCode = FName();

	DialogInfos = NpcInfo->DefaultDialogInfo;
	CurrentDialogIndex = 0;

	// 대화 내용 표시
	ShowDialog(CurrentDialogIndex);

	// 퀘스트 정보 초기화
	InitializeQuestInfos();

	// 퀘스트 수락 / 거절 버튼 숨기기
	SetButtonVisibility(Button_Rejection, false);
	SetButtonVisibility(Button_Accept, false);

	// 퀘스트 수락 버튼 이벤트 비우기
	OnAcceptButtonClickedOneshot.Clear();
}

void UNpcDialog::ChangeDialogInfo(FNpcDialogInfo newDialogInfo, bool bIsQuestDlg)
{
	// 대화 내용 변경
	DialogInfos = newDialogInfo;

	bIsQuestDialog = bIsQuestDlg;

	SetKeyboardFocus();

	// 대화 초기화
	CurrentDialogIndex = 0;
	ShowDialog(CurrentDialogIndex);

	AcceptableQuestCode = FName();
}

void UNpcDialog::CreateShopWnd()
{
	if (IsValid(NpcShopWnd)) return;

	InitializeDialog();

	FString contextString;
	FShopInfo* shopInfo = DT_ShopInfo->FindRow<FShopInfo>(NpcInfo->ShopCode, contextString);

	NpcShopWnd = Cast<UNpcShopWnd>(
		GetManager(UPlayerManager)->GetPlayerController()->GetWidgetControllerWidget()->CreateWnd(BP_NpcShopWnd, true));

	// 인벤토리 창을 띄웁니다.
	NpcShopWnd->FloatingInventoryWnd();
	NpcShopWnd->InitializeNpcShop(shopInfo);

	NpcShopWnd->OnWndClosedEvent.AddLambda(
		[this](UClosableWnd* closableWnd)
		{
			NpcShopWnd = nullptr;
		});
}

void UNpcDialog::OnQuestButtonClicked()
{
	if (IsValid(NpcQuestListWnd)) return;

	NpcQuestListWnd = Cast<UNpcQuestListWnd>(
		GetManager(UPlayerManager)->GetPlayerController()->GetWidgetControllerWidget()->CreateWnd(BP_NpcQuestListWnd));

	NpcQuestListWnd->NpcDialog = this;

	// 퀘스트 정보 갱신
	NpcQuestListWnd->UpdateQuestInfo(OrderableQuests, QuestInProgress);

	// 퀘스트 리스트 갱신
	NpcQuestListWnd->UpdateQuestList();


	NpcQuestListWnd->OnWndClosedEvent.AddLambda(
		[this](UClosableWnd* closableWnd)
		{
			NpcQuestListWnd = nullptr;
		});
}

void UNpcDialog::OnExitButtonClicked()
{
	auto widgetController = GetManager(UPlayerManager)->GetPlayerController()->GetWidgetControllerWidget();

	if (OnDlgClosed.IsBound())
		OnDlgClosed.Broadcast();

	widgetController->CloseChildWidget(this);

	// 상점 창 닫기
	if (IsValid(NpcShopWnd))
		widgetController->CloseWnd(false, NpcShopWnd);
}

void UNpcDialog::OnNextDialogButtonClicked()
{
	// 현재 표시중인 대화가 마지막 대화라면 다음 대화를 표시하지 않습니다.
	if (DialogInfos.DialogText.Num() - 1 <= CurrentDialogIndex)
		return;

	ShowDialog(++CurrentDialogIndex);

}

void UNpcDialog::OnAcceptButtonClicked()
{
	OnAcceptButtonClickedOneshot.Broadcast();
	OnAcceptButtonClickedOneshot.Clear();

	FString contextString;
	FQuestInfo* questInfo = DT_QuestInfo->FindRow<FQuestInfo>(AcceptableQuestCode, contextString);
	
	GetManager(UPlayerManager)->GetPlayerInfo()->AddProgressQuest(AcceptableQuestCode, *questInfo);

	LOG(TEXT("Quest Start! [%s]"), * AcceptableQuestCode.ToString());

	InitializeDialog();
}