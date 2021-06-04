#pragma once

#include "CoreMinimal.h"
#include "NpcDialogInfo.generated.h"

USTRUCT(BlueprintType)
struct ARPG_API FNpcDialogInfo
{
	GENERATED_USTRUCT_BODY()

public :
	// 대화 문자열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = "true"))
	TArray<FText> DialogText;

};
