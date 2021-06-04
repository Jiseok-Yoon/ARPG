#include "EnemyCharacter.h"

#include "Single/GameInstance/RPGGameInst.h"

#include "Actor/Controller/EnemyController/EnemyController.h"

#include "Component/CharacterWidget/CharacterWidgetComponent.h"

#include "Widget/CharacterWidget/EnemyCharacterWidget/EnemyCharacterWidget.h"

#include "BehaviorTree/BehaviorTree.h"

AEnemyCharacter::AEnemyCharacter()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMY_DATA(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EnemyData.DT_EnemyData'"));
	if (DT_ENEMY_DATA.Succeeded()) DT_EnemyData = DT_ENEMY_DATA.Object;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MINO(
		TEXT("SkeletalMesh'/Game/Resources/Enemies/Mino/Mesh/SK_Mino.SK_Mino'"));
	if (SK_MINO.Succeeded()) GetMesh()->SetSkeletalMesh(SK_MINO.Object);

	static ConstructorHelpers::FClassFinder<UEnemyCharacterWidget> BP_ENEMY_CHARACTER_WIDGET(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/CharacterWidget/BP_EnemyCharacterWidget.BP_EnemyCharacterWidget_C'"));
	if (BP_ENEMY_CHARACTER_WIDGET.Succeeded()) CharacterWidgetClass = BP_ENEMY_CHARACTER_WIDGET.Class;

	CharacterWidget = CreateDefaultSubobject<UCharacterWidgetComponent>(TEXT("CHARACTER_WIDGET"));
	CharacterWidget->SetupAttachment(GetRootComponent());

	// 사용하는 AI Controller 를 설정합니다.
	AIControllerClass = AEnemyController::StaticClass();

	// 컨트롤러 생성 방식을 정의합니다.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	BehaviorTree = nullptr;

	// 팀을 설정합니다.
	SetGenericTeamId(ETeam::Enemy);
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);



}


void AEnemyCharacter::InitializeEnemyCharacter(FName enemyCode)
{
	EnemyCode = enemyCode;

	// 적 코드를 이용하여 적 정보를 얻습니다.
	FString contextString;
	EnemyData = DT_EnemyData->FindRow<FEnemyData>(EnemyCode, contextString);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyCharacter"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	/// - SetCollisionProfileName(InCollisionProfileName) : 컬리전 프로파일(Collision Preset)을 설정합니다.


	// 적 정보를 정상적으로 읽지 못했다면
	if (EnemyData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyCharacter.cpp :: %d LINE :: EnemyData is null (EnemyCode Is %s)"),
			__LINE__, *EnemyCode.ToString());
		return;
	}

	// 캐릭터 위젯 초기화
	CharacterWidget->InitializeCharacterWidget(CharacterWidgetClass, FVector2D(150.0f, 60.0f));
	CharacterWidget->SetWidgetHeight((EnemyData->CapsuleHalfHeight * 2.0f) + 10.0f);
	CharacterWidget->GetCharacterWidget()->SetName(EnemyData->EnemyName);

	// 최대 이동 속력 설정
	GetCharacterMovement()->MaxWalkSpeed = EnemyData->MaxMoveSpeed;

	// 적 캡슐 크기 설정
	GetCapsuleComponent()->SetCapsuleHalfHeight(EnemyData->CapsuleHalfHeight);
	GetCapsuleComponent()->SetCapsuleRadius(EnemyData->CapsuleRadius);

	// 스켈레탈 메시 설정
	USkeletalMesh* skeletalMesh = Cast<USkeletalMesh>(GetManager(FStreamableManager)->LoadSynchronous(EnemyData->SkeletalMeshPath));
	/// - LoadSynchronous(target) : target 에 해당하는 애셋을 동기 방식으로 로드하여 UObject* 형식으로 반환합니다.
	GetMesh()->SetSkeletalMesh(skeletalMesh);

	// 스켈레탈 메시 위치 / 회전 설정
	GetMesh()->SetRelativeLocationAndRotation(
		FVector::DownVector * 88.0f,
		FRotator(0.0f, -90.0f, 0.0f));

	// BehaviorTree 설정
	BehaviorTree = Cast<UBehaviorTree>(
		GetManager(FStreamableManager)->LoadSynchronous(EnemyData->UseBehaviorTreeAssetPath));

	if (IsValid(BehaviorTree))
		Cast<AEnemyController>(GetController())->RunBehaviorTree(BehaviorTree);

	// 애님 인스턴스 클래스 설정
	GetMesh()->SetAnimInstanceClass(EnemyData->AnimClass);

	
}
