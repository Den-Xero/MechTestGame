#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHGAME_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;




private:
	//Current health
	UPROPERTY(EditAnywhere, Category = "Actor attributes")
	float Health = 100;
	UPROPERTY(EditAnywhere, Category = "Actor attributes")
	float MaxHealth = 100;
	
	
public:
	/**
	* Getters and setters
	 */
	void ReceiveDamage(const float Damage);
	float GetHealthPercent() const;
		
};
