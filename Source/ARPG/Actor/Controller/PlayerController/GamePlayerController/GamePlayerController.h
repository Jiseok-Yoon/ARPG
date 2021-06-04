#pragma once

#include "CoreMinimal.h"
#include "Actor/Controller/PlayerController/BasePlayerController.h"
#include "GamePlayerController.generated.h"

UCLASS()
class ARPG_API AGamePlayerController : public ABasePlayerController
{
	GENERATED_BODY()

protected :
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;

private :
	void RegisterToggleEvent();

private :
	// MouseX, MouseY 입력에 바인딩될 메서드
	void MouseXInput(float axis);
	void MouseYInput(float axis);
	
};
