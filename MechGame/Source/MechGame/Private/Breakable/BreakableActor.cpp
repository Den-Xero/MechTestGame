#include "Breakable/BreakableActor.h"

#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"

ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();

	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::OnChaosBreak);
}

void ABreakableActor::SpawnTreasure()
{
	if(UWorld* World = GetWorld(); World && TreasureBPClasses.Num() > 0)
	{
		const int32 SelectedTreasure = FMath::RandRange(0, TreasureBPClasses.Num() - 1);
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		World->SpawnActor<ATreasure>(TreasureBPClasses[SelectedTreasure], Location, GetActorRotation());
	}
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	if(bBroken) return;
	bBroken = true;
	SpawnTreasure();
}

void ABreakableActor::OnChaosBreak(const FChaosBreakEvent& BreakEvent)
{
	SpawnTreasure();
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

