#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterMovementHelperComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UCharacterMovementHelperComponent : public UActorComponent
{
	GENERATED_BODY()

private :
	class APlayerCharacter* PlayerCharacter;

	float WalkSpeed;
	float RunSpeed;

public:	
	UCharacterMovementHelperComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public :
	bool IsInAir() const;

public :
	void RunKeyPressed();
	void RunKeyReleased();

	void JumpKeyPressed();

	void InputHorizontal(float axis);
	void InputVertical(float axis);


		
};
