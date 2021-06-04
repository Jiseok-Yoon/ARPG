#include "RPGGameInst.h"

#include "Single/PlayerManager/PlayerManager.h"

void URPGGameInst::RegisterManagerClass(TSubclassOf<UManagerClass> managerClass)
{
	UManagerClass* managerClassInstance = NewObject<UManagerClass>(this, managerClass, 
		NAME_None, EObjectFlags::RF_MarkAsRootSet);
	/// - 생성된 객체를 GC 가 해제하지 못하도록 합니다.

	managerClassInstance->InitManagerClass();

	ManagerClasses.Add(managerClass->GetName(), managerClassInstance);
}

void URPGGameInst::Init()
{
	Super::Init();

	RegisterManagerClass(UPlayerManager::StaticClass());
}

void URPGGameInst::Shutdown()
{
	for (auto pair : ManagerClasses)
	{
		// nullptr 확인
		if (!pair.Value) return;

		// 해당 객체가 GC 에 의해 해제된 객체인지 확인합니다.
		if (!pair.Value->IsValidLowLevel()) return;

		pair.Value->ShutdownManagerClass();

		// 객체 소멸
		pair.Value->ConditionalBeginDestroy();
	}
	ManagerClasses.Empty();

	Super::Shutdown();
}
