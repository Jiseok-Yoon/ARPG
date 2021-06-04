#include "NpcDialog.h"

#include "Actor/Controller/PlayerController/BasePlayerController.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"
#include "Widget/ClosableWnd/NpcShopWnd/NpcShopWnd.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

UNpcDialog::UNpcDialog(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UClosableWnd> BP_CLOSABLE_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/BP_ClosableWnd.BP_ClosableWnd_C'"));
	if (BP_CLOSABLE_WND.Succeeded()) BP_ClosableWnd = BP_CLOSABLE_WND.Class;

	static ConstructorHelpers::FClassFinder<UNpcShopWnd> BP_NPC_SHOP_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/NpcShopWnd/BP_NpcShopWnd.BP_NpcShopWnd_C'"));
	if (BP_NPC_SHOP_WND.Succeeded()) BP_NpcShopWnd = BP_NPC_SHOP_WND.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SHOP_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ShopInfo.DT_ShopInfo'"));
	if (DT_SHOP_INFO.Succeeded()) DT_ShopInfo = DT_SHOP_INFO.Object;
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
	else if (InKeyEvent.GetKey() == EKeys::Q)
	{
		GetManager(UPlayerManager)->GetPlayerController()->
			GetWidgetControllerWidget()->CreateWnd(BP_ClosableWnd);
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

}

void UNpcDialog::SetNextDialogButtonVisibility(bool bVisible)
{
	Button_Next->SetVisibility(
		bVisible ?
		ESlateVisibility::Visible :
		ESlateVisibility::Collapsed);
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
		SetNextDialogButtonVisibility(false);
	}
	else
	{
		// 다음 대화 버튼 표시
		SetNextDialogButtonVisibility(true);
	}


}

void UNpcDialog::InitializeDialog()
{
	bIsLastDialog = false;

	DialogInfos = NpcInfo->DefaultDialogInfo;
	CurrentDialogIndex = 0;

	// 대화 내용 표시
	ShowDialog(CurrentDialogIndex);
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

void UNpcDialog::OnExitButtonClicked()
{
	if (OnDlgClosed.IsBound())
		OnDlgClosed.Broadcast();

	GetManager(UPlayerManager)->GetPlayerController()->GetWidgetControllerWidget()->CloseChildWidget(this);
}

void UNpcDialog::OnNextDialogButtonClicked()
{
	// 현재 표시중인 대화가 마지막 대화라면 다음 대화를 표시하지 않습니다.
	if (DialogInfos.DialogText.Num() - 1 <= CurrentDialogIndex)
		return;

	ShowDialog(++CurrentDialogIndex);

}