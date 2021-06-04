#include "PlayerInteractComponent.h"

#include "Component/InteractableArea/InteractableAreaComponent.h"

UPlayerInteractComponent::UPlayerInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerInteractComponent::SortByDistance()
{
	if (InteractableComponents.Num() < 2) return;

	InteractableComponents.Sort(
		[this](const UInteractableAreaComponent& A, const UInteractableAreaComponent& B)
		{
			// 플레이어 캐릭터의 위치를 저장합니다.
			FVector ownerLocation = GetOwner()->GetActorLocation();

			FVector aLocation = A.GetOwner()->GetActorLocation();
			FVector bLocation = B.GetOwner()->GetActorLocation();

			float aDist = FVector::DistXY(ownerLocation, aLocation);
			float bDist = FVector::DistXY(ownerLocation, bLocation);

			return aDist > bDist;
		});

}

void UPlayerInteractComponent::AddInteractable(UInteractableAreaComponent* newInteractable)
{
	if (!InteractableComponents.Contains(newInteractable))
	{
		InteractableComponents.Add(newInteractable);
		SortByDistance();
	}
	/// TArrayInst.Contains(item) : TArrayInst 에 item 과 일치하는 값을 갖는 요소가 
	/// 존재하는지 확인합니다.
	/// 존재한다면 true
}

void UPlayerInteractComponent::RemoveInteractable(UInteractableAreaComponent* removeInteractable)
{
	if (InteractableComponents.Contains(removeInteractable))
	{
		InteractableComponents.Remove(removeInteractable);
		SortByDistance();
	}
	/// - TArrayInst.Remove(item) : TArrayInst 에서 item 과 일치하는 값을 갖는 요소를 제거합니다.

}

void UPlayerInteractComponent::TryInteraction()
{
	// 상호작용 가능한 객체가 존재하지 않다면 실행 X
	if (InteractableComponents.Num() == 0) return;

	InteractableComponents[0]->StartInteraction();


}

