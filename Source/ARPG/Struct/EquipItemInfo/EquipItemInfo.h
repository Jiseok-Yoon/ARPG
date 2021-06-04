#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enum/PartsType.h"
#include "EquipItemInfo.generated.h"

USTRUCT(BlueprintType)
struct ARPG_API FEquipItemInfo :
	public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public :
	// 장착 가능 부위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "부위")
	EPartsType PartsType;

	// Mesh 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FSoftObjectPath MeshPath;

	// 함께 사용되는 Mesh 경로
	/// - 장갑 : 왼쪽 손에 장착되는 장갑 Mesh 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FSoftObjectPath SetMeshPath;


};
