#include "EnemySpawnerComponent.h"

#include "Actor/EnemySpawnRegion/EnemySpawnRegion.h"
#include "Actor/Character/EnemyCharacter/EnemyCharacter.h"

#include "Struct/EnemySpawnRegionInfo/EnemySpawnRegionInfo.h"

UEnemySpawnerComponent::UEnemySpawnerComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMY_SPAWN_REGION_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EnemySpawnRegionInfo.DT_EnemySpawnRegionInfo'"));
	if (DT_ENEMY_SPAWN_REGION_INFO.Succeeded()) DT_EnemySpawnRegionInfo = DT_ENEMY_SPAWN_REGION_INFO.Object;

	BoxExtent *= 10.0f;

	LineTraceStartHeight = BoxExtent.Z;
	/// - BoxExtent : 육면체 크기(X, Y, Z) / 2 의 값을 나타냅니다.

	SpawnPointXCount = SpawnPointYCount = 4;
	CurrentEnemyCount = 0;

	SpawnRegionInfo = nullptr;
}

void UEnemySpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	// 스폰 지역 코드를 얻습니다.
	FString contextString;
	FName regionCode = Cast<AEnemySpawnRegion>(GetOwner())->GetRegionCode();
	SpawnRegionInfo = DT_EnemySpawnRegionInfo->FindRow<FEnemySpawnRegionInfo>(regionCode, contextString);

	if (SpawnRegionInfo == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("EnemySpawnerComponent.cpp :: %d LINE :: SpawnRegionInfo is nullptr! (regionCode was %s)"),
			__LINE__, *regionCode.ToString());
		return;
	}

	// 스폰 지점을 초기화합니다.
	InitializeSpawnPoints();


	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindUObject(this, &UEnemySpawnerComponent::RandomSpawn);
	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 1.0f, true);
	RandomSpawn();
}

void UEnemySpawnerComponent::InitializeSpawnPoints()
{

	// 그리드를 이루는 작은 사각형들의 크기를 얻습니다.
	float gridYSize = ((BoxExtent.Y * 2) / SpawnPointYCount);
	float gridXSize = ((BoxExtent.X * 2) / SpawnPointXCount);

	FVector pivot = (GetComponentLocation() - BoxExtent);
	pivot.Z = 0.0f;

	for (int32 y = 0; y < SpawnPointYCount; ++y)
	{
		TArray<FVector> points;

		for (int32 x = 0; x < SpawnPointXCount; ++x)
		{
			// 스폰 지점으로 사용될 작은 사각형들의 중심 위치를 얻습니다.
			FVector spawnPoint = pivot +
				(GetOwner()->GetActorForwardVector() * ((gridXSize * x) + (gridXSize * 0.5f))) +
				(GetOwner()->GetActorRightVector() * ((gridYSize * y) + (gridYSize * 0.5f)));
			
			points.Add(spawnPoint);
		}

		SpawnPoints.Add(points);
	}
}

void UEnemySpawnerComponent::RandomSpawn()
{
	if (SpawnRegionInfo == nullptr) return;
	if (SpawnRegionInfo->EnemyCode.Num() == 0) return;
	if (SpawnRegionInfo->MaxEnemyCount == CurrentEnemyCount) return;


	int32 randomEnemyIndex = FMath::RandRange(
		0, SpawnRegionInfo->EnemyCode.Num() - 1);

	// 적 스폰
	SpawnEnemy(SpawnRegionInfo->EnemyCode[randomEnemyIndex]);
	++CurrentEnemyCount;
}

void UEnemySpawnerComponent::SpawnEnemy(FName enemyCode)
{
	// 스폰시킬 지점을 얻기 위해 라인 트레이싱을 시작시킬 위치를 저장할 변수
	FVector lineTraceStartLocation;

	// 라인트레이싱 시작 위치를 얻습니다.
	lineTraceStartLocation = GetRandomSpawnablePoint();

	// 라인트레이싱 시작 높이를 설정합니다.
	lineTraceStartLocation.Z = GetOwner()->GetActorLocation().Z + LineTraceStartHeight;

	// 라인트레이싱 감지 액터를 저장할 배열
	TArray<AActor*> hitActors;

	// 라인트레이싱 감지 결과를 저장할 변수
	FHitResult hitResult;

	if (UKismetSystemLibrary::LineTraceSingleByProfile(
		GetWorld(),
		lineTraceStartLocation,
		lineTraceStartLocation + (FVector::DownVector * (LineTraceStartHeight * 2.0f)),
		FName(TEXT("SpawnEnemy")),
		true,
		hitActors,
		EDrawDebugTrace::ForDuration,
		hitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		10.0f))
	{
		// 라인트레이싱 결과 위치를 저장합니다.
		FVector spawnLocation = hitResult.Location + (FVector::UpVector * (LineTraceStartHeight * 2.0f));

		// 적 캐릭터 생성
		AEnemyCharacter* newEnemyCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(
			spawnLocation, FRotator::ZeroRotator);
		newEnemyCharacter->InitializeEnemyCharacter(enemyCode);
	}
}

FVector UEnemySpawnerComponent::GetRandomSpawnablePoint() const
{
	int32 randomYIndex = FMath::RandRange(0, SpawnPointYCount - 1);
	int32 randomXIndex = FMath::RandRange(0, SpawnPointXCount - 1);

	return SpawnPoints[randomYIndex][randomXIndex];
}
