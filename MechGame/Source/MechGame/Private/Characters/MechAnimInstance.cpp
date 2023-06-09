#include "Characters/MechAnimInstance.h"

#include "Characters/PlayerMechCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMechAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MechCharacter = Cast<APlayerMechCharacter>(TryGetPawnOwner());
	if(MechCharacter)
	{
		MechCharacterMovement = MechCharacter->GetCharacterMovement();
	}
}

void UMechAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(MechCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MechCharacterMovement->Velocity);
		IsFalling = MechCharacterMovement->IsFalling();
		CharacterStates = MechCharacter->GetCharacterStates();
	}
}
