#include "BasePlayerController.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

#include "Component/WndToggler/WndTogglerComponent.h"

#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"

ABasePlayerController::ABasePlayerController()
{
	static ConstructorHelpers::FClassFinder<UWidgetControllerWidget> BP_WIDGET_CONTROLLER_WIDGET(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WidgetController/BP_WidgetController.BP_WidgetController_C'"));
	if (BP_WIDGET_CONTROLLER_WIDGET.Succeeded()) BP_WidgetController = BP_WIDGET_CONTROLLER_WIDGET.Class;
	else { UE_LOG(LogTemp, Error, TEXT("BasePlayerController.cpp :: %d LINE :: BP_WIDGET_CONTROLLER_WIDGET is not loaded!"), __LINE__); }

	WndToggler = CreateDefaultSubobject<UWndTogglerComponent>(TEXT("WND_TOGGLER"));
		
	DefaultInputMode = EInputModeType::IM_GameOnly;
	bDefaultCursorVisibility = false;
}

void ABasePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	GetManager(UPlayerManager)->RegisterPlayer(this, Cast<APlayerCharacter>(aPawn));

	WidgetControllerWidget = CreateWidget<UWidgetControllerWidget>(this, BP_WidgetController);
	WidgetControllerWidget->InitializeWidgetControllerWidget(this);
	WidgetControllerWidget->AddToViewport();

}

void ABasePlayerController::ChangeViewTarget(AActor* targetActor, float blendTime)
{
	SetViewTargetWithBlend(targetActor, blendTime);
}

void ABasePlayerController::ResetViewTarget(float blendTime)
{
	auto playerCharacter = GetManager(UPlayerManager)->GetPlayerCharacter();

	if (!IsValid(playerCharacter)) return;

	SetViewTargetWithBlend(playerCharacter, blendTime);
}
