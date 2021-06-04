#pragma once

#include "ARPG.h"
#include "Components/BoxComponent.h"
#include "EnemySpawnerComponent.generated.h"

UCLASS()
class ARPG_API UEnemySpawnerComponent : public UBoxComponent
{
	GENERATED_BODY()
	
private :
	class UDataTable* DT_EnemySpawnRegionInfo;



private :
	// 스폰 지점이 X 축 방향으로 몇개 위치하도록 할 것인지를 지정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region Settings", meta = (AllowPrivateAccess = "true"))
	int32 SpawnPointXCount;

	// 스폰 지점이 Y 축 방향으로 몇개 위치하도록 할 것인지를 지정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region Settings", meta = (AllowPrivateAccess = "true"))
	int32 SpawnPointYCount;

	// 라인트레이스 시작 높이를 지정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region Settings", meta = (AllowPrivateAccess = "true"))
	float LineTraceStartHeight;

	// 적 스폰 지역 정보를 나타냅니다.
	struct FEnemySpawnRegionInfo* SpawnRegionInfo;

	// 스폰 지역들을 나타냅니다.
	TArray<TArray<FVector>> SpawnPoints;

	// 현재 생성된 적 개체수
	int32 CurrentEnemyCount;

public :
	UEnemySpawnerComponent();

	virtual void BeginPlay() override;

private :
	// 스폰 지점들을 초기화합니다.
	void InitializeSpawnPoints();

	void RandomSpawn();

	void SpawnEnemy(FName enemyCode);

	// 랜덤한 스폰 위치를 얻습니다.
	FVector GetRandomSpawnablePoint() const;



};
