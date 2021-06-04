#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enum/CharacterClassType.h"
#include "CharacterClassInfo.generated.h"

USTRUCT(BlueprintType)
struct ARPG_API FCharacterClassInfo : 
	public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// 캐릭터 클래스를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "클래스")
	ECharacterClassType ClassType;

	// 기본으로 장착되는 장비 아이템 코드들을 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기타")
	TArray<FName> DefaultEquipItemCodes;
};
