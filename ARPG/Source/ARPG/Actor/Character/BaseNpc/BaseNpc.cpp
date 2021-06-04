#include "BaseNpc.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Actor/Controller/PlayerController/BasePlayerController.h"

#include "Widget/NpcDialog/NpcDialog.h"
#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"

#include "Component/InteractableArea/InteractableAreaComponent.h"

ABaseNpc::ABaseNpc()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UNpcDialog> BP_NPC_DIALOG(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/NpcDialog/BP_NpcDialog.BP_NpcDialog_C'"));
	if (BP_NPC_DIALOG.Succeeded()) BP_NpcDialog = BP_NPC_DIALOG.Class;
	else { UE_LOG(LogTemp, Error, TEXT("ABaseNpc.cpp :: %d LINE :: BP_NPC_DIALOG is not loaded!"), __LINE__); }

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_NPC_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_NpcInfo.DT_NpcInfo'"));
	if (DT_NPC_INFO.Succeeded()) DT_NpcInfo = DT_NPC_INFO.Object;

	InteractableArea = CreateDefaultSubobject<UInteractableAreaComponent>(TEXT("INTERACTABLE_AREA"));
	InteractableArea->SetupAttachment(GetRootComponent());

	InteractCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("INTERACT_CAMERA"));
	InteractCamera->SetupAttachment(GetRootComponent());
	InteractCamera->SetRelativeLocationAndRotation(
		FVector::ForwardVector * 180.0f,
		FRotator(0.0f, 180.0f, 0.0f));

	InteractCamera->SetConstraintAspectRatio(true);

}

void ABaseNpc::BeginPlay()
{
	Super::BeginPlay();

	// 상호작용 시작 시 실행할 내용 정의
	InteractableArea->OnInteractionStarted.AddLambda(
		[this](UInteractableAreaComponent* interactableArea)
		{
			auto playerController = GetManager(UPlayerManager)->GetPlayerController();

			// 뷰 타깃을 변경합니다
			playerController->ChangeViewTarget(this);

			// NpcDialog 위젯을 생성합니다.
			UNpcDialog* newNpcDialog = CreateWidget<UNpcDialog>(playerController, BP_NpcDialog);

			// Npc Dialog 객체 초기화
			newNpcDialog->InitializeNpcDialog(GetNpcInfo());

			// 위젯 띄우기
			playerController->GetWidgetControllerWidget()->AddChildWidget(newNpcDialog);

			// 닫기 버튼 이벤트
			newNpcDialog->OnDlgClosed.AddLambda(
				[=]()
				{
					// 위젯이 닫힐 때 뷰 타깃을 초기화합니다.
					playerController->ResetViewTarget();
				});
		});
}

void ABaseNpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseNpc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FNpcInfo* ABaseNpc::GetNpcInfo(FName npcCode)
{
	FString contextString;
	return DT_NpcInfo->FindRow<FNpcInfo>(npcCode, contextString);
}

FNpcInfo* ABaseNpc::GetNpcInfo()
{
	// Npc 정보가 로드되지 않았다면 NpcCode 를 이용하여 Npc 정보를 읽습니다.
	if (NpcInfo.IsEmpty())
		NpcInfo = *GetNpcInfo(NpcCode);

	// 읽어온 정보가 잘못 되었다면 경고 띄우기
	if (NpcInfo.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("NpcInfo.cpp :: %d LINE :: NpcInfo Is Empty! exp = (NpcCode is %s)"),
			__LINE__, *NpcCode.ToString());
	}

	return &NpcInfo;
}

