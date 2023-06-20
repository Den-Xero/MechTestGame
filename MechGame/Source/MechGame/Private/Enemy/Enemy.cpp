#include "Enemy/Enemy.h"

#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MechGame/DebugMacros.h"


AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Healthbar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if(HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(1.f);
	}
}

void AEnemy::PlayHitReactMontage(const FName& SectionName) const
{
	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());

	}
	return DamageAmount;
}

double AEnemy::GetAngle(const FVector& Side, const FVector& ToHit) const
{
	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Side, ToHit)));
}

bool AEnemy::IsEitherSideOf45(const float& SideAngle)
{
	return SideAngle >= -45.f && SideAngle <= 45.f;
}

void AEnemy::DirectionalHitReaction(const FVector& ImpactPoint) const
{
	const FVector Front  = GetActorForwardVector();
	const FVector Back = -Front;
	const FVector Right = GetActorRightVector();
	const FVector Left = -Right;
	
	//Lowing the impact point to enemy's location z
	const FVector ImpactedLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactedLowered - GetActorLocation()).GetSafeNormal();

	//Forward * ToHit = |Forward||ToHit|cos(theta)
	//Forward * ToHit = cos(theta)
	//Acos(cos(theta)) = theta
	//Convert from radians to degrees
	const float FrontAngle = GetAngle(Front, ToHit);
	const float BackAngle = GetAngle(Back, ToHit);
	const float RightAngle = GetAngle(Right, ToHit);
	const float LeftAngle = GetAngle(Left, ToHit);

	//Find the angle that is within 45 either side.
	FName SectionName;
	if (IsEitherSideOf45(FrontAngle)) SectionName = FName("FromFront");
	else if (IsEitherSideOf45(BackAngle)) SectionName = FName("FromBack");
	else if (IsEitherSideOf45(RightAngle)) SectionName = FName("FromRight");
	else if (IsEitherSideOf45(LeftAngle)) SectionName = FName("FromLeft");

	PlayHitReactMontage(SectionName);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	DirectionalHitReaction(ImpactPoint);

	if(HitSounds)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSounds, ImpactPoint);
	}

	if(HitParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticleSystem, ImpactPoint);
	}
	
}

