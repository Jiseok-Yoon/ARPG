#include "ZoomableSpringArmComponent.h"

UZoomableSpringArmComponent::UZoomableSpringArmComponent()
{
	// 기본적으로 줌 기능을 사용하도록 합니다.
	bUseZoom = true;

	// 줌 최소, 최대 거리를 설정합니다.
	SetArmLengthMinMax(100.0f, 400.0f);
}

void UZoomableSpringArmComponent::BeginPlay()
{
	Super::BeginPlay();

	// 목표 거리를 설정합니다.
	TargetArmLength = ArmLengthTarget =
		(MaxArmLength - MinArmLength) * 0.5f;
}

void UZoomableSpringArmComponent::TickComponent(float dt, 
	ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(dt, tickType, thisTickFunction);

	SmoothZoomSpringArm(dt);
}

void UZoomableSpringArmComponent::SmoothZoomSpringArm(float dt)
{
	TargetArmLength = FMath::FInterpTo(
		TargetArmLength, ArmLengthTarget,
		dt, 5.0f);
}

void UZoomableSpringArmComponent::ZoomCamera(float axis)
{
	// 줌 기능을 사용하지 않는다면 카메라 줌이 이루어질수 없도록 합니다.
	if (!bUseZoom) return;

	ArmLengthTarget += (axis * -5.0f);
	ArmLengthTarget = FMath::Clamp(
		ArmLengthTarget, MinArmLength, MaxArmLength);

}
