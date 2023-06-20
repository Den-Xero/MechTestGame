#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

struct FChaosBreakEvent;
class UCapsuleComponent;
class ATreasure;
class UGeometryCollectionComponent;

UCLASS()
class MECHGAME_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	void SpawnTreasure();
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	UPROPERTY(EditAnywhere, Category = "Spawnable objects")
	TArray<TSubclassOf<ATreasure>> TreasureBPClasses;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> StopPlayerCapsuleComponent;
	UFUNCTION()
	virtual void OnBreak(const FChaosBreakEvent& BreakEvent);
	UPROPERTY(EditAnywhere, Category = "Spawnable objects")
	float AddToZ = 0;
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;
	bool bBroken = false;
	
public:	
	

};
