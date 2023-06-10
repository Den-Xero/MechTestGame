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
	if(ActionStates != EActionStates::EAS_Unoccupied) return;
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

bool APlayerMechCharacter::ShouldDisarm() const
{
	return CharacterStates != ECharacterStates::ECS_Unequipped &&
		CharacterStates != ECharacterStates::ECS_NoWeapon &&
			ActionStates == EActionStates::EAS_Unoccupied &&
				EquipMontage;
}

bool APlayerMechCharacter::ShouldArm() const
{
	return CharacterStates == ECharacterStates::ECS_Unequipped &&
		ActionStates == EActionStates::EAS_Unoccupied &&
			EquippedWeapon &&
				EquipMontage;
}

void APlayerMechCharacter::DisarmArm(FName Section, ECharacterStates States)
{
	PlayEquipMontage(Section);
	CharacterStates =States;
	ActionStates = EActionStates::EAS_Equipping;
}

void APlayerMechCharacter::PickUpWeapon(AWeapon* OverlappingWeapon)
{
	OverlappingWeapon->Equip(GetMesh(), TEXT("RightHandSocket"));
	CharacterStates = ECharacterStates::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = OverlappingWeapon;
}

void APlayerMechCharacter::Interact()
{
	if(AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		PickUpWeapon(OverlappingWeapon);
	}
	else if (ShouldDisarm())
	{
		DisarmArm( FName("Unequipped"), ECharacterStates::ECS_Unequipped);
	}
	else if (ShouldArm())
	{
		DisarmArm( FName("Equip"), ECharacterStates::ECS_EquippedOneHandedWeapon);
	}
}

void APlayerMechCharacter::PlayAttackMontage() const
{
	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 2);
		FName SectionName = FName();
		switch(Selection)
		{
		case 0:
			SectionName = TEXT("Attack1");
			break;
		case 1:
			SectionName = TEXT("Attack2");
			break;
		case 2:
			SectionName = TEXT("Attack3");
			break;
		default:
			SectionName = TEXT("Attack1");
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void APlayerMechCharacter::PlayEquipMontage(FName SectionName) const
{
	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(EquipMontage);
		
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void APlayerMechCharacter::EquipEnd()
{
	ActionStates = EActionStates::EAS_Unoccupied;
}

void APlayerMechCharacter::Arm()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->AttachToSocket(GetMesh(), TEXT("RightHandSocket"));
	}
	
}

void APlayerMechCharacter::Disarm()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->AttachToSocket(GetMesh(), TEXT("SpineSocket"));
	}
	
}

void APlayerMechCharacter::AttackEnd()
{
	ActionStates = EActionStates::EAS_Unoccupied;
}

bool APlayerMechCharacter::CanAttack() const
{
	return ActionStates == EActionStates::EAS_Unoccupied &&
		CharacterStates != ECharacterStates::ECS_Unequipped &&
			CharacterStates != ECharacterStates::ECS_NoWeapon;
}

void APlayerMechCharacter::Attack()
{
	if(CanAttack())
	{
		PlayAttackMontage();
		ActionStates = EActionStates::EAS_Attacking;
		
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
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerMechCharacter::Interact);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerMechCharacter::Attack);
	}
}

void APlayerMechCharacter::Jump()
{
	if(ActionStates != EActionStates::EAS_Unoccupied) return;
	Super::Jump();
}

