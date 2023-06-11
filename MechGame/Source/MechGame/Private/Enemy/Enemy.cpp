#include "Enemy/Enemy.h"
#include "Components/CapsuleComponent.h"
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
	
	

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
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

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DRAWSPHEREWITHCOLOUR(ImpactPoint, FColor::Emerald);
	

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
	const float FrontAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Front, ToHit)));
	const float BackAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Back, ToHit)));
	const float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Right, ToHit)));
	const float LeftAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Left, ToHit)));

	//Find the angle that is within 45 either side.
	FName SectionName;
	if(FrontAngle >= -45.f && FrontAngle <= 45.f) SectionName = FName("FromFront");
	else if(BackAngle >= -45.f && BackAngle <= 45.f) SectionName = FName("FromBack");
	else if(RightAngle >= -45.f && RightAngle <= 45.f) SectionName = FName("FromRight");
	else if(LeftAngle >= -45.f && LeftAngle <= 45.f) SectionName = FName("FromLeft");

	PlayHitReactMontage(SectionName);
}

