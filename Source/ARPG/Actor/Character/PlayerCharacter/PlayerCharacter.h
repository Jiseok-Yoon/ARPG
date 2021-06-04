#pragma once

#include "Actor/Character/BaseCharacter.h"
#include "Enum/PartsType.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class ARPG_API APlayerCharacter final : 
	public ABaseCharacter
{
	GENERATED_BODY()

private :
	TSubclassOf<class UPlayerCharacterAnimInst> BP_PlayerCharacterAnimInst;

private :
	UPROPERTY()
	TMap<EPartsType, class USkeletalMeshComponent*> Parts;

private :
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UZoomableSpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCharacterMovementHelperComponent * CharacterMovementHelper;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UPlayerInteractComponent * PlayerInteract;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USkillControllerComponent * SkillController;

	class USkeletalMeshComponent* HeadMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* HairMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* BeardMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* TopMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* BottomMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* RGloveMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* LGloveMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* ShoesMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;

	


public :
	APlayerCharacter();

public :
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public :
	void InitializeMeshs();

public :
	FORCEINLINE class UCharacterMovementHelperComponent* GetCharacterMovementHelper() const
	{ return CharacterMovementHelper; }

	FORCEINLINE class UPlayerInteractComponent* GetPlayerInteract() const
	{ return PlayerInteract; }

	FORCEINLINE class UZoomableSpringArmComponent* GetSpringArm() const
	{ return SpringArm; }

	FORCEINLINE class USkillControllerComponent* GetSkillController() const
	{ return SkillController; }

	FORCEINLINE TMap<EPartsType, class USkeletalMeshComponent*>& GetParts()
	{ return Parts; }

private:
	void RegularAttack();

};
