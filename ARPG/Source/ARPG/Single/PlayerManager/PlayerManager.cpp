#include "PlayerManager.h"

#include "Actor/Controller/PlayerController/BasePlayerController.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

void UPlayerManager::RegisterPlayer(ABasePlayerController* newPlayerController, 
	APlayerCharacter* newPlayerCharacter)
{
	PlayerController = newPlayerController;
	PlayerCharacter = newPlayerCharacter;
}

void UPlayerManager::InitManagerClass()
{
	PlayerInventory = NewObject<UPlayerInventory>(this, UPlayerInventory::StaticClass(),
		NAME_None, EObjectFlags::RF_MarkAsRootSet);
}

void UPlayerManager::ShutdownManagerClass()
{
	if (!PlayerInventory->IsValidLowLevel()) return;
	PlayerInventory->ConditionalBeginDestroy();
}
