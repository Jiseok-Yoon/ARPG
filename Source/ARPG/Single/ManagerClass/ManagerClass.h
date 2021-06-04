#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ManagerClass.generated.h"

#ifndef MANAGER_INSTANCE
#define MANAGER_INSTANCE
#define GetManager(ManagerClassType) (GetGameInst(GetWorld())->GetManagerClass<ManagerClassType>())
#endif

UCLASS(Abstract)
class ARPG_API UManagerClass : 
	public UObject
{
	GENERATED_BODY()

public :
	FORCEINLINE virtual void InitManagerClass() {};
	FORCEINLINE virtual void ShutdownManagerClass() {};
	
};
