#include "Items/Weapons/Weapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
	WeaponDamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Damage Box "));
	WeaponDamageBox->SetupAttachment(GetRootComponent());
	WeaponDamageBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponDamageBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	WeaponDamageBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box trace start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box trace end"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if(WeaponDamageBox)
	{
		WeaponDamageBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
	}
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::AttachToSocket(USceneComponent* InParent, FName InSocketName) const
{
	const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	AttachToSocket(InParent, InSocketName);
	ItemStates = EItemStates::EIS_Equipped;
	if(PickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation());
		if(Sphere)
		{
			Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		
	}
}


void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
}

void AWeapon::EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(BoxTraceStart && BoxTraceEnd)
	{
		const FVector Start = BoxTraceStart->GetComponentLocation();
		const FVector End = BoxTraceEnd->GetComponentLocation();
		IgnoreActors.AddUnique(this);
		FHitResult HitResult;
		UKismetSystemLibrary::BoxTraceSingle(
			this, Start, End,
			FVector(5.f, 5.f, 5.f),
			BoxTraceStart->GetComponentRotation(),
			TraceTypeQuery1, false,
			IgnoreActors,EDrawDebugTrace::None,
			HitResult, true);

		if(AActor* HitActor = HitResult.GetActor())
		{
			if(IHitInterface* HitInterface = Cast<IHitInterface>(HitActor))
			{
				HitInterface->Execute_GetHit(HitActor, HitResult.ImpactPoint);
			}
			IgnoreActors.AddUnique(HitActor);

			CreateField(HitResult.ImpactPoint);
		}
	}
}

void AWeapon::SetWeaponDamageBoxCollisionType(ECollisionEnabled::Type NewCollisionType) const
{
	if(WeaponDamageBox) WeaponDamageBox->SetCollisionEnabled(NewCollisionType);
}
