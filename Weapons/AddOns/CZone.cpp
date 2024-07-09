#include "CZone.h"
#include "Kismet/GameplayStatics.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

ACZone::ACZone()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");

	Capsule->SetCapsuleHalfHeight(44);
	Capsule->SetCapsuleRadius(44);

	this->InitialLifeSpan = 5.0f;

	// HitData정보
	{
		HitData.Launch = 0.0f;
		HitData.Power  = 5.0f;
		CHelpers::GetAsset<UAnimMontage>(&HitData.Montage, "AnimMontage'/Game/Character/Montages/Fist/Fist_Hitted_Montage_Player.Fist_Hitted_Montage_Player'");
	}
}

void ACZone::BeginPlay()
{
	Super::BeginPlay();

	FindTarget();
	{
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACZone::OnComponentBeginOverlap);
		Capsule->OnComponentEndOverlap.AddDynamic(this, &ACZone::OnComponentEndOverlap);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACZone::SendMessage, DamageInterval, true);
	}
	

	SetActorLocation(Target->GetActorLocation());

}

void ACZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return;

	while (1)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
			break;
	}
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}

void ACZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	
	
}

void ACZone::FindTarget()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), EnemyTag, Enemies);

	float ShortestDistance = FLT_MAX;
	AActor* NearestEnemy = nullptr;

	for (AActor* Enemy : Enemies)
	{
		float DistanceToEnemy = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
		if (DistanceToEnemy < ShortestDistance)
		{
			ShortestDistance = DistanceToEnemy;
			NearestEnemy = Enemy;
		}
	}

	if (NearestEnemy != nullptr)
	{
		Target = NearestEnemy;
	}
}


void ACZone::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* chararcter = Cast<ACharacter>(OtherActor);
	if (!!chararcter)
		Hit.AddUnique(chararcter);
}
void ACZone::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);
	ACharacter* chararcter = Cast<ACharacter>(OtherActor);
	if (!!chararcter)
		Hit.Remove(chararcter);
}
void ACZone::SendMessage()
{
	if (Hit.Num() == 0)
		return;
	for (int32 i = Hit.Num() - 1; i >= 0; i--)
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hit[i]);
}