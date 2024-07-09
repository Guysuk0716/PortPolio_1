#include "CSword.h"
#include "Kismet/GameplayStatics.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

ACSword::ACSword()
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

void ACSword::BeginPlay()
{
	Super::BeginPlay();

	FindTarget();
	{
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACSword::OnComponentBeginOverlap);
		Capsule->OnComponentEndOverlap.AddDynamic(this, &ACSword::OnComponentEndOverlap);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACSword::SendMessage, DamageInterval, true);
	}
	

	

}

void ACSword::EndPlay(const EEndPlayReason::Type EndPlayReason)
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

void ACSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (Target == nullptr)
	//{
	//   FindTarget();
	//   if (Target == nullptr) return;  // 타겟을 찾지 못한 경우 업데이트 종료
	//}

	// 유도 시간 업데이트
	HomingTimeElapsed += DeltaTime;

	// 유도 시간이 남아있는 경우 회전
	if (HomingTimeElapsed <= HomingDuration)
	{
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Direction.Normalize();

		FRotator TargetRotation = Direction.Rotation();
		FRotator CurrentRotation = GetActorRotation();

		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotateSpeed);
		SetActorRotation(NewRotation);
	}
	// 유도 시간이 끝난 경우 회전 중지 및 이동
	else
	{
		// 유도가 종료되면 회전 중지
		// 회전 중지를 위해 현재 회전 방향을 유지
		FRotator CurrentRotation = GetActorRotation();
		SetActorRotation(CurrentRotation);

		// 유도가 종료되면 이전 방향으로 직진
		FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime);
		SetActorLocation(NewLocation);
	}
	
	
}

void ACSword::FindTarget()
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


void ACSword::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* chararcter = Cast<ACharacter>(OtherActor);
	if (!!chararcter)
		Hit.AddUnique(chararcter);
}
void ACSword::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);
	ACharacter* chararcter = Cast<ACharacter>(OtherActor);
	if (!!chararcter)
		Hit.Remove(chararcter);
}
void ACSword::SendMessage()
{
	if (Hit.Num() == 0)
		return;
	for (int32 i = Hit.Num() - 1; i >= 0; i--)
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hit[i]);
}