#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Enum/CommandLogType.h"

#include "CommandLogTextWidget.generated.h"

UCLASS()
class ARPG_API UCommandLogTextWidget : public UUserWidget
{
	GENERATED_BODY()

private :
	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Log;

public:
	void SetLogLevel(ECommandLog::Type logLevel);

	void SetText(FString str);
	void SetText(FText text);


	
	
};
