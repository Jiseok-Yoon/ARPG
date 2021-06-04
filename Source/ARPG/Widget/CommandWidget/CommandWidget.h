#pragma once

#include "ARPG.h"
#include "Blueprint/UserWidget.h"

#include "Enum/CommandType.h"

#include "Struct/CommandLog/CommandLog.h"

#include "CommandWidget.generated.h"

UCLASS()
class ARPG_API UCommandWidget final : 
	public UUserWidget
{
	GENERATED_BODY()


private :
	TSubclassOf<class UCommandLogTextWidget> BP_CommandLogTextWidget;
	class UDataTable* DT_ItemInfo;

private :
	UPROPERTY()
	FCommandLog AddedLogs;

private :
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* EditableTextBox_Input;
	
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox * VerticalBox_CommandLogs;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox * ScrollBox_Logs;
public :
	UCommandWidget(const FObjectInitializer& objInitializer);

protected :
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private :
	void AddLog(FString log, ECommandLog::Type logType);
	void AddLog(FText log, ECommandLog::Type logType);

	void ShowDefaultMessage(FString defaultMessage);
	void ShowWarningMessage(FString warningMessage);
	void ShowErrorMessage(FString errorMessage);

	// 입력한 명령어 타입을 반환하는 메서드
	/// - inputText : 입력한 문자열을 전달합니다.
	ECommand::Type GetCommandType(FString inputText);

private :
	UFUNCTION()
	void OnCommandCommitted(const FText& Text, ETextCommit::Type CommitMethod);


	
};
