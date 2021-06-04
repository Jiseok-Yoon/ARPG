#pragma once

#include "Actor/Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ARPG_API APlayerCharacter final : 
	public ABaseCharacter
{
	GENERATED_BODY()

private :
	TSubclassOf<class UPlayerCharacterAnimInst> BP_PlayerCharacterAnimInst;

private :
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UZoomableSpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCharacterMovementHelperComponent * CharacterMovementHelper;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UPlayerInteractComponent * PlayerInteract;
	


public :
	APlayerCharacter();

public :
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public :
	FORCEINLINE class UCharacterMovementHelperComponent* GetCharacterMovementHelper() const
	{ return CharacterMovementHelper; }

	FORCEINLINE class UPlayerInteractComponent* GetPlayerInteract() const
	{ return PlayerInteract; }

	FORCEINLINE class UZoomableSpringArmComponent* GetSpringArm() const
	{ return SpringArm; }

};
