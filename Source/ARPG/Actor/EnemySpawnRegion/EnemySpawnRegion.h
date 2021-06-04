#pragma once

#include "ARPG.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnRegion.generated.h"


// 적 스폰 지역을 나타내는 액터
UCLASS()
class ARPG_API AEnemySpawnRegion final : 
	public AActor
{
	GENERATED_BODY()

private :
	class UTexture2D* T_EnemySpawnRegionIcon;

private :
	// 적 스폰 지점 코드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Region Code")
	FName RegionCode;

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UEnemySpawnerComponent* EnemySpawner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBillboardComponent * BillboardRegionIcon;

public:	
	AEnemySpawnRegion();

public :
	FORCEINLINE FName GetRegionCode() const
	{ return RegionCode; }

};
