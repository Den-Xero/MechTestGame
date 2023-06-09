#include "Characters/PlayerMechCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"

APlayerMechCharacter::APlayerMechCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 650.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = TEXT("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = TEXT("head");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
}

void APlayerMechCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem ->AddMappingContext(MechMappingContext, 0);
		}
	}
}

void APlayerMechCharacter::Move(const FInputActionValue& Value)
{
	const FVector2d MoveAxisValue = Value.Get<FVector2d>();
	if(GetController() && MoveAxisValue != FVector2d::Zero())
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FVector ForwardVector = FRotationMatrix(FRotator(0.f, Rotation.Yaw, 0.f)).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardVector, MoveAxisValue.Y);
		const FVector RightVector = FRotationMatrix(FRotator(0.f, Rotation.Yaw, 0.f)).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightVector, MoveAxisValue.X);
	}
}

void APlayerMechCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisValue = Value.Get<FVector2d>();
	if(GetController())
	{
		AddControllerPitchInput(LookAxisValue.Y);
		AddControllerYawInput(LookAxisValue.X);
	}
}

void APlayerMechCharacter::Interact()
{
	if(AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		OverlappingWeapon->Equip(GetMesh(), TEXT("RightHandSocket"));
		CharacterStates = ECharacterStates::ECS_EquippedOneHandedWeapon;
	}
}

void APlayerMechCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerMechCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerMechCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerMechCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerMechCharacter::Jump);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerMechCharacter::Interact);
	}
}

void APlayerMechCharacter::Jump()
{
	Super::Jump();
}

