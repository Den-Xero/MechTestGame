#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

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
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ItemMesh;
};

template <typename Type>
Type AItem::Avg(Type first, Type second)
{
	return (first + second) / 2;
}
