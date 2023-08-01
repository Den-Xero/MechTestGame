#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class AAIController;
class UHealthBarComponent;
class UAttributeComponent;

UCLASS()
class MECHGAME_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	double GetAngle(const FVector& Side, const FVector& ToHit) const;
	static bool IsEitherSideOf45(const float& SideAngle);
	void DirectionalHitReaction(const FVector& ImpactPoint) const;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Sounds)
	TObjectPtr<USoundBase> HitSounds;
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	TObjectPtr<UParticleSystem> HitParticleSystem;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
	
	void SetHealthBarVis(bool IsVisible) const;

	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;
	

	bool InRange(const AActor* Target, double Radius) const;
	/**
	 * Navigation
	 */
	TObjectPtr<AAIController> EnemyController;
	//Current point to patrol to.
	UPROPERTY(EditInstanceOnly, Category = " AI Navigation")
	TObjectPtr<AActor> PatrolTarget;
	int PatrolNumber;
	//List of patrol points
	UPROPERTY(EditInstanceOnly, Category = " AI Navigation")
	TArray<AActor*> PatrolPoints;
	UPROPERTY(EditAnywhere, Category = " AI Navigation")
	double PatrolRadius = 200.f;
	UPROPERTY(EditAnywhere, Category = " AI Navigation")
	float MinWait = 5.f;
	UPROPERTY(EditAnywhere, Category = " AI Navigation")
	float MaxTime = 10.f;
	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();
	
	/**
	 * Play montage functions
	 */
	void PlayHitReactMontage(const FName& SectionName) const;
	




	void Die();
	void MoveToTarget(const AActor* Target);
	void CheckCombatTarget();
	void Patrol();

private:
	/**
	 * Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> HitReactMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> DeathMontage;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;


	bool MoveDoOnce = false;
	bool TestOnce = false;
	
	
public:	
	

};
