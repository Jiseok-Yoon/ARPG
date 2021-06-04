#pragma once

#include "ARPG.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS(Abstract)
class ARPG_API ABaseCharacter : 
	public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
