#include "BasePlayerController.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

#include "Component/WndToggler/WndTogglerComponent.h"
#include "Component/PlayerBehaviorBroadcast/PlayerBehaviorBroadcastComponent.h"

#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"
#include "Widget/CommandWidget/CommandWidget.h"

#include "Components/CanvasPanelSlot.h"

ABasePlayerController::ABasePlayerController()
{
	static ConstructorHelpers::FClassFinder<UWidgetControllerWidget> BP_WIDGET_CONTROLLER_WIDGET(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WidgetController/BP_WidgetController.BP_WidgetController_C'"));
	if (BP_WIDGET_CONTROLLER_WIDGET.Succeeded()) BP_WidgetController = BP_WIDGET_CONTROLLER_WIDGET.Class;
	else { UE_LOG(LogTemp, Error, TEXT("BasePlayerController.cpp :: %d LINE :: BP_WIDGET_CONTROLLER_WIDGET is not loaded!"), __LINE__); }

	static ConstructorHelpers::FClassFinder<UCommandWidget> BP_COMMAND_WIDGET(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/CommandWidget/BP_CommandWidget.BP_CommandWidget_C'"));
	if (BP_COMMAND_WIDGET.Succeeded()) BP_CommandWidget = BP_COMMAND_WIDGET.Class;

	PlayerBehaviorBroadcast = CreateDefaultSubobject<UPlayerBehaviorBroadcastComponent>(TEXT("PLAYER_BH_BROADCAST"));
	WndToggler = CreateDefaultSubobject<UWndTogglerComponent>(TEXT("WND_TOGGLER"));
		
	DefaultInputMode = EInputModeType::IM_GameOnly;
	bDefaultCursorVisibility = false;
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("OpenCommandWidget"), EInputEvent::IE_Pressed, this, &ABasePlayerController::ToggleCommandWidget);
}

void ABasePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	GetManager(UPlayerManager)->RegisterPlayer(this, Cast<APlayerCharacter>(aPawn));

	WidgetControllerWidget = CreateWidget<UWidgetControllerWidget>(this, BP_WidgetController);
	WidgetControllerWidget->InitializeWidgetControllerWidget(this);
	WidgetControllerWidget->AddToViewport();

}

void ABasePlayerController::OpenCommandWidget()
{
	if (IsValid(CommandWidget)) return;

	CommandWidget = CreateWidget<UCommandWidget>(this, BP_CommandWidget);
	WidgetControllerWidget->AddChildWidget(CommandWidget, EInputModeType::IM_Default, true, 450.0f, 400.0f);
	Cast<UCanvasPanelSlot>(CommandWidget->Slot)->SetPosition(FVector2D(50.0f, 50.0f));
}

void ABasePlayerController::CloseCommandWidget()
{
	if (IsValid(CommandWidget))
	{
		WidgetControllerWidget->CloseChildWidget(CommandWidget);
		CommandWidget = nullptr;
	}
}

void ABasePlayerController::ToggleCommandWidget()
{
	if (IsValid(CommandWidget)) CloseCommandWidget();
	else OpenCommandWidget();
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
