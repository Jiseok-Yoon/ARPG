#pragma once

#include "CoreMinimal.h"

#include "Single/ManagerClass/ManagerClass.h"
#include "Single/PlayerManager/PlayerInventory/PlayerInventory.h"

#include "Struct/PlayerCharacterInfo/PlayerCharacterInfo.h"



#include "PlayerManager.generated.h"

UCLASS()
class ARPG_API UPlayerManager final : 
	public UManagerClass
{
	GENERATED_BODY()

private :
	class UDataTable* DT_CharacterClassInfo;
	class UDataTable* DT_EquipItemInfo;

private :
	UPROPERTY()
	UPlayerInventory* PlayerInventory;

	UPROPERTY()
	class ABasePlayerController* PlayerController;

	UPROPERTY()
	class APlayerCharacter * PlayerCharacter;

	UPROPERTY()
	FPlayerCharacterInfo PlayerInfo;

public :
	UPlayerManager();

public :
	// 플레이어 컨트롤러와 캐릭터를 등록합니다.
	void RegisterPlayer(class ABasePlayerController* newPlayerController,
		class APlayerCharacter* newPlayerCharacter);

public:
	virtual void InitManagerClass() override;
	virtual void ShutdownManagerClass() override;

public :
	// 캐릭터를 초기화합니다.
	void InitializePlayerCharacter();

public :
	// 인벤토리 객체를 반환합니다.
	FORCEINLINE UPlayerInventory* GetPlayerInventory() const
	{ return PlayerInventory; }

	FORCEINLINE class ABasePlayerController* GetPlayerController() const
	{ return PlayerController; }

	FORCEINLINE class APlayerCharacter* GetPlayerCharacter() const
	{ return PlayerCharacter; }

	FORCEINLINE FPlayerCharacterInfo* GetPlayerInfo()
	{ return &PlayerInfo; }
	
};
