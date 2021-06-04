#pragma once

#include "ARPG.h"
#include "GameFramework/Character.h"
#include "Struct/NpcInfo/NpcInfo.h"
#include "BaseNpc.generated.h"

UCLASS()
class ARPG_API ABaseNpc : public ACharacter
{
	GENERATED_BODY()

private :
	class UDataTable* DT_NpcInfo;

	TSubclassOf<class UNpcDialog> BP_NpcDialog;

protected :
	UPROPERTY(VisibleAnywhere)
	class UInteractableAreaComponent * InteractableArea;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent * InteractCamera;

	FNpcInfo NpcInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Npc Code")
	FName NpcCode;

public:
	ABaseNpc();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private :
	FNpcInfo* GetNpcInfo(FName npcCode);

public :
	FNpcInfo* GetNpcInfo();

	FORCEINLINE class UInteractableAreaComponent* GetInteractableArea() const
	{ return InteractableArea; }



};
