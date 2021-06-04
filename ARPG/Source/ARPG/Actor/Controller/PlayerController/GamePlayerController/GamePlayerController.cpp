#include "GamePlayerController.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"

#include "Component/WndToggler/WndTogglerComponent.h"


void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("OpenPlayerInventoryWnd"), EInputEvent::IE_Pressed,
		GetWndToggler(), &UWndTogglerComponent::ToggleWnd<UPlayerInventory>);

	InputComponent->BindAxis(TEXT("MouseX"), this, &AGamePlayerController::MouseXInput);
	InputComponent->BindAxis(TEXT("MouseY"), this, &AGamePlayerController::MouseYInput);
}

void AGamePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	RegisterToggleEvent();
}

void AGamePlayerController::RegisterToggleEvent()
{

#pragma region UPlayerInventory
	FToggleEvent playerInventoryWndToggleEvent;
	playerInventoryWndToggleEvent.BindLambda(
		[this]() { 
			GetManager(UPlayerManager)->GetPlayerInventory()->
				ToggleInventoryWnd(GetWidgetControllerWidget()); 
		});
	GetWndToggler()->RegisterToggleEvent<UPlayerInventory>(playerInventoryWndToggleEvent);
#pragma endregion

}

void AGamePlayerController::MouseXInput(float axis)
{
	// 컨트롤러에 axis 만큼 Yaw 회전을 시킵니다.
	AddYawInput(axis);
}

void AGamePlayerController::MouseYInput(float axis)
{
	// 컨트롤러에 axis 만큼 Pitch 회전을 시킵니다.
	AddPitchInput(axis);
}
