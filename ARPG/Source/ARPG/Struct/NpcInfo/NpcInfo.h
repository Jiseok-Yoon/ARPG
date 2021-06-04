#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "Struct/NpcDialogInfo/NpcDialogInfo.h"

#include "NpcInfo.generated.h"

USTRUCT(BlueprintType)
struct ARPG_API FNpcInfo : 
	public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// Npc 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText NpcName;

	// 기본 대화 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNpcDialogInfo DefaultDialogInfo;

	// 상점 코드
	UPROPERTY(EditAnywhere, BLueprintReadWrite)
	FName ShopCode;

public :
	FORCEINLINE bool IsEmpty() const
	{ return NpcName.IsEmpty(); }

	FORCEINLINE bool UseShop() const
	{ return !ShopCode.IsNone(); }
};
