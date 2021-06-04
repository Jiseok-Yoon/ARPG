#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInst.generated.h"

UCLASS()
class ARPG_API UPlayerCharacterAnimInst final : 
	public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float VelocityLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsInAir;


public :
	virtual void NativeUpdateAnimation(float dt) override;
	
};
