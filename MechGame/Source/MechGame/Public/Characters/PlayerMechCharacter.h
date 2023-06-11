#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Characters/CharacterTypes.h"
#include "PlayerMechCharacter.generated.h"

class AWeapon;
class AItem;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;




UCLASS()
class MECHGAME_API APlayerMechCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerMechCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

protected:
	virtual void BeginPlay() override;
	bool CanAttack() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MechMappingContext;

	/**
	 * Callback functions
	 */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	bool ShouldDisarm() const;
	bool ShouldArm() const;
	void DisarmArm(FName Section, ECharacterStates States);
	void PickUpWeapon(AWeapon* OverlappingWeapon);
	void Interact();
	void Attack();
	

	/**
	 * Play montage functions
	 */
	void PlayAttackMontage() const;
	void PlayEquipMontage(const FName& SectionName) const;
	
	/**
	 * montage notify functions
	 */
		UFUNCTION(BlueprintCallable)
    	void EquipEnd();
    	UFUNCTION(BlueprintCallable)
    	void Arm();
    	UFUNCTION(BlueprintCallable)
    	void Disarm();
    	UFUNCTION(BlueprintCallable)
    	void ChangeAttackCollisionType(ECollisionEnabled::Type NewCollisionType);
		UFUNCTION(BlueprintCallable)
        void AttackEnd();
private:
	ECharacterStates CharacterStates = ECharacterStates::ECS_NoWeapon;
	EActionStates ActionStates = EActionStates::EAS_Unoccupied;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	TObjectPtr<UGroomComponent> Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	TObjectPtr<UGroomComponent> Eyebrows;
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<AWeapon> EquippedWeapon{nullptr};



	/**
	 * Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> EquipMontage;


public:
	/**
	 * Getters and setters
	 */
	FORCEINLINE void SetOverlappingItem(AItem* Item) {OverlappingItem = Item;}
	FORCEINLINE ECharacterStates GetCharacterStates() const {return CharacterStates;}
};
