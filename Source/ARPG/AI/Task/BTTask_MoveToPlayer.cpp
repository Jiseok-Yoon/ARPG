#include "BTTask_MoveToPlayer.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Actor/Controller/EnemyController/EnemyController.h"

#include "BehaviorTree/BlackboardComponent.h"


using namespace EBTNodeResult;

EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(
	UBehaviorTreeComponent& ownerComponent, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComponent, nodeMemory);

	AEnemyController* enemyController = Cast<AEnemyController>(ownerComponent.GetAIOwner());
	APlayerCharacter* playerCharacter = GetManager(UPlayerManager)->GetPlayerCharacter();

	if (!IsValid(playerCharacter))
		return Failed;

	//enemyController->GetBlackboardComponent()->SetValueAsVector(
	//	FName(TEXT("TargetPosition")),
	//	playerCharacter->GetActorLocation());

	return Succeeded;
}