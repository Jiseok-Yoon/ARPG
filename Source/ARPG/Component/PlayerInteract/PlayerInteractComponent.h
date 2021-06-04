#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UPlayerInteractComponent : public UActorComponent
{
	GENERATED_BODY()

private :
	// 상호작용 가능한 객체들을 저장할 배열
	TArray <class UInteractableAreaComponent*> InteractableComponents;

public:	
	UPlayerInteractComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private :
	// 플레이어 캐릭터와 가까운 순서대로 정렬합니다.
	void SortByDistance();

public :
	// 상호작용 가능한 객체로 추가합니다.
	void AddInteractable(class UInteractableAreaComponent* newInteractable);

	// 상호작용 가능한 객체에서 제외시킵니다.
	void RemoveInteractable(class UInteractableAreaComponent* removeInteractable);

	// 상호작용을 시도합니다.
	void TryInteraction();

		
};
