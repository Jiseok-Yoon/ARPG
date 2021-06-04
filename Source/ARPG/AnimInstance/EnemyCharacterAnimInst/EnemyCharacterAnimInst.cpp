#include "EnemyCharacterAnimInst.h"

#include "Actor/Character/EnemyCharacter/EnemyCharacter.h"

void UEnemyCharacterAnimInst::NativeUpdateAnimation(float dt)
{
	Super::NativeUpdateAnimation(dt);

	AEnemyCharacter* owner = Cast<AEnemyCharacter>(TryGetPawnOwner());

	if (!IsValid(owner)) return;

	VelocityLength = owner->GetVelocity().Size();
}
