#include "GamePlayerController.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"
#include "Widget/ClosableWnd/PlayerEquipItemWnd/PlayerEquipItemWnd.h"

#include "Component/WndToggler/WndTogglerComponent.h"


void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("OpenPlayerInventoryWnd"), EInputEvent::IE_Pressed,
		GetWndToggler(), &UWndTogglerComponent::ToggleWnd<UPlayerInventory>);

	InputComponent->BindAction(TEXT("OpenPlayerEquipItemWnd"), EInputEvent::IE_Pressed,
		GetWndToggler(), &UWndTogglerComponent::ToggleWnd<UPlayerEquipItemWnd>);

	InputComponent->BindAxis(TEXT("MouseX"), this, &AGamePlayerController::MouseXInput);
	InputComponent->BindAxis(TEXT("MouseY"), this, &AGamePlayerController::MouseYInput);
}

void AGamePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// -TODO- Test 용 Code 임.
	/// 나중에 캐릭터 클래스가 추가되는 경우 제거되어야 함.
	GetManager(UPlayerManager)->InitializePlayerCharacter();
	///--------------------------------------------------///

	RegisterToggleEvent();
}

void AGamePlayerController::RegisterToggleEvent()
{

#pragma region UPlayerInventory
	FToggleEvent playerInventoryWndToggleEvent;
	playerInventoryWndToggleEvent.BindLambda(
		[this]() { 
			GetManager(UPlayerManager)->GetPlayerInventory()->
				ToggleInventoryWnd(GetWidgetControllerWidget(), true); 
		});
	GetWndToggler()->RegisterToggleEvent<UPlayerInventory>(playerInventoryWndToggleEvent);
#pragma endregion

#pragma region UPlayerEquipItemWnd
	FToggleEvent playerEquipItemWndToggleEvent;

	playerEquipItemWndToggleEvent.BindLambda(
		[this]()
		{
			GetManager(UPlayerManager)->GetPlayerInventory()->
				ToggleEquipItemWnd(GetWidgetControllerWidget());
		});

	GetWndToggler()->RegisterToggleEvent<UPlayerEquipItemWnd>(playerEquipItemWndToggleEvent);
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
