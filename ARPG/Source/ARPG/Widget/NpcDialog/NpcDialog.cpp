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

	// �� ������ ��Ŀ���� �����ϵ��� �մϴ�.
	bIsFocusable = true;
	SetKeyboardFocus();
	// Ű �Է��� ��Ŀ�� �� ���¿��� �̷������ �մϴ�.

	Button_Exit->OnClicked.AddDynamic(this, &UNpcDialog::OnExitButtonClicked);
	Button_Next->OnClicked.AddDynamic(this, &UNpcDialog::OnNextDialogButtonClicked);
	Button_Shop->OnClicked.AddDynamic(this, &UNpcDialog::CreateShopWnd);
	Button_GoToFirst->OnClicked.AddDynamic(this, &UNpcDialog::InitializeDialog);
}

FReply UNpcDialog::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::R)
	{
		// ���� ��ȭ ������ ǥ���մϴ�.
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

	// Npc �̸� ǥ��
	Text_NpcName->SetText(NpcInfo->NpcName);

	// ���̾�α� �ʱ�ȭ
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
	// ��� ������ ��ȭ �ؽ�Ʈ�� �������� �ʴ´ٸ�
	if (DialogInfos.DialogText.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NpcDialog.cpp :: %d LINE :: Usable Dialog Count Is Zero!"), __LINE__);
		return;
	}

	// ǥ���Ϸ��� ��ȭ �ε����� �迭 ������ �ʰ��Ѵٸ�
	else if (DialogInfos.DialogText.Num() <= newDialogIndex)
	{
		UE_LOG(LogTemp, Error,
			TEXT("NpcDialog.cpp :: %d LINE :: [Warning] Out Of Range! newDialogIndex is Changed (%d -> %d)"),
			__LINE__,
			newDialogIndex,
			DialogInfos.DialogText.Num() - 1);

		// ǥ���Ϸ��� ��ȭ �ε����� ������ ��ȭ �ε����� �����մϴ�.
		newDialogIndex = (DialogInfos.DialogText.Num() - 1);
	}

	// �ؽ�Ʈ ����
	Text_Dialog->SetText(DialogInfos.DialogText[newDialogIndex]);

	// ������ ��ȭ���� Ȯ���մϴ�.
	bIsLastDialog = (DialogInfos.DialogText.Num() - 1) == newDialogIndex;

	// ������ ��ȭ���
	if (bIsLastDialog)
	{
		// ���� ��ư �����
		SetNextDialogButtonVisibility(false);
	}
	else
	{
		// ���� ��ȭ ��ư ǥ��
		SetNextDialogButtonVisibility(true);
	}


}

void UNpcDialog::InitializeDialog()
{
	bIsLastDialog = false;

	DialogInfos = NpcInfo->DefaultDialogInfo;
	CurrentDialogIndex = 0;

	// ��ȭ ���� ǥ��
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

	// �κ��丮 â�� ���ϴ�.
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
	// ���� ǥ������ ��ȭ�� ������ ��ȭ��� ���� ��ȭ�� ǥ������ �ʽ��ϴ�.
	if (DialogInfos.DialogText.Num() - 1 <= CurrentDialogIndex)
		return;

	ShowDialog(++CurrentDialogIndex);

}