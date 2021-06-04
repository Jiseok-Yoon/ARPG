#pragma once

#include "ARPG.h"
#include "Components/SphereComponent.h"
#include "InteractableAreaComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FInteractEvent, class UInteractableAreaComponent *)

UCLASS()
class ARPG_API UInteractableAreaComponent : public USphereComponent
{
	GENERATED_BODY()
	
public :
	// 상호작용이 시작되었을 때 호출되는 대리자
	FInteractEvent OnInteractionStarted;

public :
	virtual void BeginPlay() override;

public :
	// 상호작용을 시작합니다.
	void StartInteraction();

private :
	UFUNCTION()
	void OnPlayerCharacterDetected(
		UPrimitiveComponent*	OverlappedComponent,
		AActor*					OtherActor,
		UPrimitiveComponent*	OtherComp,
		int32					OtherBodyIndex,
		bool					bFromSweep,
		const FHitResult&		SweepResult);

	UFUNCTION()
	void OnPlayerCharacterLost(
		UPrimitiveComponent*	OverlappedComponent,
		AActor*					OtherActor,
		UPrimitiveComponent*	OtherComp,
		int32					OtherBodyIndex);

};
