#include "Items/Item.h"

#include "Characters/PlayerMechCharacter.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include  "MechGame/DebugMacros.h"



AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMexhComponent"));
	SetRootComponent(ItemMesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Syetem"));
	NiagaraSystem->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::EndSphereOverlap);
}

float AItem::TransformedSin() const
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos() const
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(APlayerMechCharacter* MechCharacter = Cast<APlayerMechCharacter>(OtherActor))
	{
		MechCharacter->SetOverlappingItem(this);
	}
	
}

void AItem::EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(APlayerMechCharacter* MechCharacter = Cast<APlayerMechCharacter>(OtherActor))
	{
		MechCharacter->SetOverlappingItem(nullptr);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	
	if(ItemStates == EItemStates::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f,0.f,TransformedSin()));
    	AddActorWorldRotation(FRotator(0.f, Rotate * DeltaTime, 0.f));
	}
	
}

