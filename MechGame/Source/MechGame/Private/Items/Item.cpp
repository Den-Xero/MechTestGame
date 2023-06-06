#include "Items/Item.h"
#include  "MechGame/DebugMacros.h"



AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	const FVector ActorLocation = GetActorLocation();
	
	DRAWSPHERE(ActorLocation)
	DRAWVECTOR(ActorLocation, ActorLocation + GetActorForwardVector() * 100.f)
	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

