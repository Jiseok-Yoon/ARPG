#include "BTTask_UpdateTrackingPosition.h"

#include "Actor/Controller/EnemyController/EnemyController.h"


using namespace EBTNodeResult;

EBTNodeResult::Type UBTTask_UpdateTrackingPosition::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyController* enemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());

	if (enemyController->UpdateTrackingPosition())
		return Succeeded;
	else return Failed;
}