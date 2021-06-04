#include "CharacterMovementHelperComponent.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

UCharacterMovementHelperComponent::UCharacterMovementHelperComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WalkSpeed = 300.0f;
	RunSpeed = 600.0f;
}

void UCharacterMovementHelperComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// 공중에서 캐릭터를 80% 제어 가능하도록 합니다.
	PlayerCharacter->GetCharacterMovement()->AirControl = 0.8f;

	// 캐릭터가 받는 중력 설정
	PlayerCharacter->GetCharacterMovement()->GravityScale = 2.5f;

	// 점프 초기 가속력 설정
	PlayerCharacter->GetCharacterMovement()->JumpZVelocity = 1000.0f;
}

void UCharacterMovementHelperComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UCharacterMovementHelperComponent::IsInAir() const
{
	return IsValid(PlayerCharacter) ? 
		PlayerCharacter->GetCharacterMovement()->IsFalling() : true;
}

void UCharacterMovementHelperComponent::RunKeyPressed()
{
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void UCharacterMovementHelperComponent::RunKeyReleased()
{
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void UCharacterMovementHelperComponent::JumpKeyPressed()
{
	PlayerCharacter->Jump();
}

void UCharacterMovementHelperComponent::InputHorizontal(float axis)
{
	// 컨트롤러 회전중 Yaw 회전만을 저장합니다.
	FRotator yawRotation(0.0f, PlayerCharacter->GetControlRotation().Yaw, 0.0f);

	// 컨트롤러 기준 오른쪽 방향을 저장합니다.
	FVector rightVector = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	// FRotationMatrix : 회전된 좌표계 정보를 저장하는 행렬을 나타냅니다.
	// GetUnitAxis(EAxis) : EAxis 축으로의 방향을 반환합니다.

	PlayerCharacter->AddMovementInput(rightVector, axis);
}

void UCharacterMovementHelperComponent::InputVertical(float axis)
{
	// 컨트롤러 회전중 Yaw 회전 얻기
	FRotator yawRotation(0.0f, PlayerCharacter->GetControlRotation().Yaw, 0.0f);

	// 컨트롤러 기준 앞 방향 저장
	FVector forwardVector = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	PlayerCharacter->AddMovementInput(forwardVector, axis);
}

