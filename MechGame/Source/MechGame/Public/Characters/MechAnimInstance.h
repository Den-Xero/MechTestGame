#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CharacterTypes.h"
#include "MechAnimInstance.generated.h"


class UCharacterMovementComponent;
class APlayerMechCharacter;

UCLASS()
class MECHGAME_API UMechAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerMechCharacter>  MechCharacter;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TObjectPtr<UCharacterMovementComponent> MechCharacterMovement;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Character States")
	ECharacterStates CharacterStates;
};
