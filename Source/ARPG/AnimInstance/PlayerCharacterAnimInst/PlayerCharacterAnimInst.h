#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInst.generated.h"

UCLASS()
class ARPG_API UPlayerCharacterAnimInst final : 
	public UAnimInstance
{
	GENERATED_BODY()

private :
	class APlayerCharacter* Owner;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float VelocityLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsInAir;


public :
	virtual void NativeUpdateAnimation(float dt) override;

private :
	UFUNCTION()
	void AnimNotify_SkillFinished();

	UFUNCTION()
	void AnimNotify_SetSkillRequestable();
	
	UFUNCTION()
	void AnimNotify_BlockSkillRequestable();

	UFUNCTION()
	void AnimNotify_MakeSkillRange();
	
	UFUNCTION()
	void AnimNotify_MakeNextSkillRange();

};
