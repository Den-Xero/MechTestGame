#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class MECHGAME_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	
	virtual void Tick(float DeltaTime) override;
	void AttachToSocket(USceneComponent* InParent, FName InSocketName) const;

	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	UPROPERTY()
	TArray<AActor*> IgnoreActors;
protected:
	virtual void BeginPlay() override;
	virtual  void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual  void EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<UBoxComponent> WeaponDamageBox;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<USceneComponent> BoxTraceStart;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<USceneComponent> BoxTraceEnd;
	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& FieldLocation);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;
private:
	

public:
	/**
	 * Getters and setters
	 */
	FORCEINLINE void SetWeaponDamageBoxCollisionType(ECollisionEnabled::Type NewCollisionType) const;

};