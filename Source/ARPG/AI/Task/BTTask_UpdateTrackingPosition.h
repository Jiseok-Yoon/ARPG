#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UpdateTrackingPosition.generated.h"

UCLASS()
class ARPG_API UBTTask_UpdateTrackingPosition : 
	public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public :
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
