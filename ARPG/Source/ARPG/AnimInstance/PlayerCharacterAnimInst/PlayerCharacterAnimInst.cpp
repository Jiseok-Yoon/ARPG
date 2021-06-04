#include "PlayerCharacterAnimInst.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

#include "Component/CharacterMovementHelper/CharacterMovementHelperComponent.h"

void UPlayerCharacterAnimInst::NativeUpdateAnimation(float dt)
{
	Super::NativeUpdateAnimation(dt);

	auto owner = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (!IsValid(owner)) return;

	VelocityLength = owner->GetVelocity().Size();

	bIsInAir = owner->GetCharacterMovementHelper()->IsInAir();
}
