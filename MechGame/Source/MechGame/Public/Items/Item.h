#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;


enum class EItemStates : uint8
{
	EIS_Hovering UMETA(DisplayName = "Hovering"),
	EIS_Equipped UMETA(DisplayName = "Equipped")
	
};


UCLASS()
class MECHGAME_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintPure)
	float TransformedSin() const;
	UFUNCTION(BlueprintPure)
	float TransformedCos() const;
	UFUNCTION()
	virtual  void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual  void EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ItemMesh;
	UPROPERTY(EditAnywhere, Category = Sounds)
	TObjectPtr<USoundBase> PickUpSound;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	EItemStates ItemStates = EItemStates::EIS_Hovering;
	
	template<typename  Type>
	static Type Avg(Type first, Type second);
private:
	UPROPERTY(VisibleInstanceOnly, Category = "Sine parameters")
	float RunningTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Sine parameters")
	float Amplitude = 0.75f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Sine parameters")
	float TimeConstant = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Rotation")
	float Rotate = 50.f;
	
};

template <typename Type>
Type AItem::Avg(Type first, Type second)
{
	return (first + second) / 2;
}
