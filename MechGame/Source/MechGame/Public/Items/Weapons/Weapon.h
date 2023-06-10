#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

UCLASS()
class MECHGAME_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	void AttachToSocket(USceneComponent* InParent, FName InSocketName);

	void Equip(USceneComponent* InParent, FName InSocketName);

protected:
	virtual  void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual  void EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


private:
	
	
	
};
