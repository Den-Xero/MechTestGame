#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

UCLASS()
class MECHGAME_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
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
	
	/**
	 * Play montage functions
	 */
	void PlayHitReactMontage(const FName& SectionName) const;

private:
	/**
	 * Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> HitReactMontage;
	
public:	
	

};
