#include "CommandWidget.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widget/CommandWidget/CommandLogTextWidget/CommandLogTextWidget.h"

#include "Struct/ItemInfo/ItemInfo.h"

#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"

using namespace ECommand;

UCommandWidget::UCommandWidget(const FObjectInitializer& objInitializer) : 
	Super(objInitializer)
{
	static ConstructorHelpers::FClassFinder<UCommandLogTextWidget> BP_COMMAND_LOG_TEXT_WIDGET(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/CommandWidget/BP_CommandLogTextWidget.BP_CommandLogTextWidget_C'"));
	if (BP_COMMAND_LOG_TEXT_WIDGET.Succeeded()) BP_CommandLogTextWidget = BP_COMMAND_LOG_TEXT_WIDGET.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ItemInfo.DT_ItemInfo'"));
	if (DT_ITEM_INFO.Succeeded()) DT_ItemInfo = DT_ITEM_INFO.Object;

}

void UCommandWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// EditableTextBox 에 엔터 입력이나, 포커싱 관련 이벤트가 발생했을 경우 호출할 메서드를 등록합니다.
	EditableTextBox_Input->OnTextCommitted.AddDynamic(this, &UCommandWidget::OnCommandCommitted);

	// Command Widget 이 포커스를 지원하도록 합니다.
	bIsFocusable = true;
	EditableTextBox_Input->SetKeyboardFocus();

	if (GetGameInst(GetWorld())->LoadJson<FCommandLog>(TEXT("CommandLog"), AddedLogs))
	{
		// 전에 입력한 명령 로그들 추가
		for (int32 i = 0; i < AddedLogs.Num(); ++i)
		{
			auto logData = AddedLogs[i];
			AddLog(logData.Get<0>(), logData.Get<1>());
		}
	}

	// 스크롤바를 최하단으로 내립니다.
	ScrollBox_Logs->ScrollToEnd();
}

void UCommandWidget::NativeDestruct()
{
	Super::NativeDestruct();

	GetGameInst(GetWorld())->SaveJson(TEXT("CommandLog"), AddedLogs);
}

void UCommandWidget::AddLog(FString log, ECommandLog::Type logType)
{ AddLog(FText::FromString(log), logType); }
void UCommandWidget::AddLog(FText log, ECommandLog::Type logType)
{
	UCommandLogTextWidget* newLogTextWidget = CreateWidget<UCommandLogTextWidget>(this, BP_CommandLogTextWidget);
	VerticalBox_CommandLogs->AddChild(newLogTextWidget);

	newLogTextWidget->SetText(log);
	newLogTextWidget->SetLogLevel(logType);

	// 스크롤바를 최하단으로 내립니다.
	ScrollBox_Logs->ScrollToEnd();
}

void UCommandWidget::ShowDefaultMessage(FString defaultMessage)
{
	auto logData = MakeTuple(defaultMessage, ECommandLog::Log_Default);
	AddLog(logData.Get<0>(), logData.Get<1>());
	AddedLogs.AddLog(logData.Get<0>(), logData.Get<1>());
}

void UCommandWidget::ShowWarningMessage(FString warningMessage)
{
	auto logData = MakeTuple(warningMessage, ECommandLog::Log_Warning);
	AddLog(logData.Get<0>(), logData.Get<1>());
	AddedLogs.AddLog(logData.Get<0>(), logData.Get<1>());
	LOG(TEXT("%s"), *warningMessage);
}

void UCommandWidget::ShowErrorMessage(FString errorMessage)
{
	auto logData = MakeTuple(errorMessage, ECommandLog::Log_Error);
	AddLog(logData.Get<0>(), logData.Get<1>());
	AddedLogs.AddLog(logData.Get<0>(), logData.Get<1>());
	UE_LOG(LogTemp, Error, TEXT("%s"), *errorMessage);
}

ECommand::Type UCommandWidget::GetCommandType(FString inputText)
{
	if (inputText.Find(TEXT("SpawnItem")) == 0) return Command_SpawnItem;
	return Command_None;
}

void UCommandWidget::OnCommandCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	LOG(TEXT("CommitMethod = %d"), CommitMethod);


	if (CommitMethod == ETextCommit::OnEnter)
	{
		FString commandStr = Text.ToString();

		switch (GetCommandType(commandStr))
		{

		case Command_SpawnItem :
		{
			// 문자열을 공백을 기준으로 나눠 저장할 배열
			TArray<FString> commandArgs;
			commandStr.ParseIntoArray(commandArgs, TEXT(" "));

			// 입력한 명령 포맷이 잘못된 경우
			if (commandArgs.Num() < 2 || commandArgs.Num() > 3)
			{
				ShowErrorMessage(TEXT("잘못된 명령 : ") + commandStr);
				ShowErrorMessage(TEXT("ex) SpawnItem [ItemCode] [Count]"));
				break;
			}

			// 아이템 코드
			FName itemCode = FName(*commandArgs[1]);

			// 아이템 정보 로드
			FString contextString;
			FItemInfo* itemInfo = DT_ItemInfo->FindRow<FItemInfo>(itemCode, contextString);

			// 아이템 정보를 찾지 못했다면
			if (itemInfo == nullptr)
			{
				ShowErrorMessage(TEXT("잘못된 아이템 코드 : ") + commandArgs[1]);
				break;
			}

			// 추가될 아이템 개수
			int32 itemCount = 1;

			// 개수까지 입력한 경우
			/// - ex) SpawnItem [ItemCode] [ItemCount]
			if (commandArgs.Num() > 2)
				itemCount = FCString::Atoi(*commandArgs[2]);
			/// - Atoi(const TCHAR * string) : string 을 int32 형식으로 변환하여 반환합니다.
			///   만약 변환 불가능한 문자열일 경우 0 을 반환합니다.
			
			// 정상적인 아이템 범위가 입력되지 않았다면
			if (itemCount < 1 || itemCount > 1000)
			{
				ShowErrorMessage(TEXT("잘못된 아이템 개수 : ") + commandArgs[2]);
				ShowErrorMessage(TEXT("아이템 개수는 1 ~ 1000 개 까지 지정할 수 있습니다."));
				break;
			}
			
			// 인벤토리에 아이템 추가
			/// - 장비 아이템이라면 슬롯 최대 개수가 1 이므로
			/// - 추가가 여러번 일어날 수 있도록 합니다.
			if (itemInfo->ItemType == EItemType::Equipment)
			{
				for (int32 i = 0; i < itemCount; ++i)
				{
					FItemSlotInfo newItem(itemCode, 1, 1);
					GetManager(UPlayerManager)->GetPlayerInventory()->AddItem(newItem);
				}
			}
			// 장비 아이템이 아니라면
			else
			{
				int32 maxSlotCount = (itemInfo->MaxSlotCount == 0) ? 1 : itemInfo->MaxSlotCount;
				FItemSlotInfo newItem(itemCode, itemCount, maxSlotCount);
				GetManager(UPlayerManager)->GetPlayerInventory()->AddItem(newItem);
			}
			break;
		}

		case Command_None :
		{
			ShowErrorMessage(TEXT("잘못된 명령 : ") + commandStr);
			break;
		}

		}

		EditableTextBox_Input->SetText(FText());
	}
	else if (CommitMethod == ETextCommit::OnCleared)
	{
		EditableTextBox_Input->SetKeyboardFocus();
	}


}
