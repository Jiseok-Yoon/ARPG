#include "CommandLogTextWidget.h"

#include "Components/TextBlock.h"

void UCommandLogTextWidget::SetLogLevel(ECommandLog::Type logLevel)
{
	const static FLinearColor defaultLogColor(0.9f, 0.9f, 0.9f, 1.0f);
	const static FLinearColor warningLogColor(1.0f, 0.6f, 0.0f, 1.0f);
	const static FLinearColor errorLogColor(1.0f, 0.1f, 0.1f, 1.0f);

	switch (logLevel)
	{
	case ECommandLog::Log_Default:
		Text_Log->SetColorAndOpacity(defaultLogColor);
		break;
	case ECommandLog::Log_Warning:
		Text_Log->SetColorAndOpacity(warningLogColor);
		break;
	case ECommandLog::Log_Error:
		Text_Log->SetColorAndOpacity(errorLogColor);
		break;
	}
}

void UCommandLogTextWidget::SetText(FString str)
{ SetText(FText::FromString(str)); }
void UCommandLogTextWidget::SetText(FText text)
{
	Text_Log->SetText(text);
}

