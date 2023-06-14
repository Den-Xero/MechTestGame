#include "Items/Treasure.h"
#include "Characters/PlayerMechCharacter.h"
#include "Kismet/GameplayStatics.h"

ATreasure::ATreasure()
{
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(APlayerMechCharacter* MechCharacter = Cast<APlayerMechCharacter>(OtherActor))
	{
		if(PickUpSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation());
		}
		Destroy();
	}
	
}
