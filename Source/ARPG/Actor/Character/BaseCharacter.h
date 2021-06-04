#pragma once

#include "ARPG.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Enum/Team.h"
#include "BaseCharacter.generated.h"

UCLASS(Abstract)
class ARPG_API ABaseCharacter : 
	public ACharacter,
	public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected :
	FGenericTeamId Team;

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE virtual void SetGenericTeamId(const FGenericTeamId& TeamID) 
	{ Team = TeamID; }
	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const 
	{ return Team; }


};
