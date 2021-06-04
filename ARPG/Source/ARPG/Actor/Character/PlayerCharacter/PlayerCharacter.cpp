#include "PlayerCharacter.h"

#include "AnimInstance/PlayerCharacterAnimInst/PlayerCharacterAnimInst.h"

#include "Component/ZoomableSpringArm/ZoomableSpringArmComponent.h"
#include "Component/CharacterMovementHelper/CharacterMovementHelperComponent.h"
#include "Component/PlayerInteract/PlayerInteractComponent.h"


APlayerCharacter::APlayerCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_TEST_MESH(
		TEXT("SkeletalMesh'/Game/Resources/PlayerCharacter/GKnight/Meshes/SK_GothicKnight_VF.SK_GothicKnight_VF'"));
	if (SK_TEST_MESH.Succeeded()) GetMesh()->SetSkeletalMesh(SK_TEST_MESH.Object);

	static ConstructorHelpers::FClassFinder<UPlayerCharacterAnimInst> BP_PLAYER_CHARACTER_ANIM_INST(
		TEXT("AnimBlueprint'/Game/Blueprints/AnimInstance/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (BP_PLAYER_CHARACTER_ANIM_INST.Succeeded()) BP_PlayerCharacterAnimInst = BP_PLAYER_CHARACTER_ANIM_INST.Class;

	// 컴포넌트 추가
	CharacterMovementHelper = CreateDefaultSubobject<UCharacterMovementHelperComponent>(TEXT("MOVEMENT_HELPER"));
	SpringArm = CreateDefaultSubobject<UZoomableSpringArmComponent>(TEXT("SPRING_ARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetConstraintAspectRatio(true);

	PlayerInteract = CreateDefaultSubobject<UPlayerInteractComponent>(TEXT("PLAYER_INTERACT"));

	// Spring Arm Component 를 루트 컴포넌트에 추가합니다.
	SpringArm->SetupAttachment(GetRootComponent());

	// Camera Component 를 SpringArm 컴포넌트에 추가합니다.
	Camera->SetupAttachment(SpringArm);

	// 컨트롤러의 회전값을 SpringArm Component 회전값으로 사용합니다.
	SpringArm->bUsePawnControlRotation = true;

	// 컨트롤러의 회전중 Yaw, Pitch 회전을 사용합니다.
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritPitch = true;

	// SpringArm 오프셋을 설정합니다.
	SpringArm->TargetOffset = FVector::UpVector * 70.0f;

	// 캐릭터 기본 위치 / 회전 설정
	GetMesh()->SetRelativeLocationAndRotation(
		FVector::DownVector * 88.0f,
		FRotator(0.0f, -90.0f, 0.0f));

	// 이 액터의 회전이 컨트롤러의 Yaw 회전을 사용하지 않도록 합니다.
	bUseControllerRotationYaw = false;

	// 이동하는 방향으로 캐릭터를 회전시킵니다.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 회전 속도를 지정합니다.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 450.0f, 0.0f);

	// 애님 인스턴스 클래스 설정
	GetMesh()->SetAnimClass(BP_PlayerCharacterAnimInst);

	Tags.Add(PLAYER_ACTOR_TAG);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed,
		GetCharacterMovementHelper(), &UCharacterMovementHelperComponent::RunKeyPressed);

	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released,
		GetCharacterMovementHelper(), &UCharacterMovementHelperComponent::RunKeyReleased);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed,
		GetCharacterMovementHelper(), &UCharacterMovementHelperComponent::JumpKeyPressed);

	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed,
		GetPlayerInteract(), &UPlayerInteractComponent::TryInteraction);


	PlayerInputComponent->BindAxis(TEXT("MouseWheel"), 
		SpringArm, &UZoomableSpringArmComponent::ZoomCamera);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"),
		GetCharacterMovementHelper(), &UCharacterMovementHelperComponent::InputHorizontal);

	PlayerInputComponent->BindAxis(TEXT("Vertical"),
		GetCharacterMovementHelper(), &UCharacterMovementHelperComponent::InputVertical);
}
