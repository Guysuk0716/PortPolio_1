// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CEnemy.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Weapons/CWeaponstructures.h"

// Sets default values
ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	




	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

}
void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	Create_DynamicMaterial(this);//
	Change_Color(this, OriginColor);
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);


}
void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get the character's velocity
	FVector Velocity = GetCharacterMovement()->Velocity;
	float Speed = Velocity.Size();

	// Log the speed
	UE_LOG(LogTemp, Warning, TEXT("Current Speed: %f"), Speed);

}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hit:   Hit(); break;
	case EStateType::Dead:  Dead(); break;
	}

}

void ACEnemy::RestoreColor()
{
	GetWorld()->GetTimerManager().ClearTimer(RestoreColor_TimerHandle);
	Change_Color(this, OriginColor);
}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetHitMode();

	//   PrintLine();
	//   this->LaunchCharacter(FVector(0, 0, 300), false, false);
	return damage;
}

void ACEnemy::ApplyFireDamage(float DotDamage, float Duration)
{
	TotalDotDamage = DotDamage * Duration;
	RemainingDotTime = Duration;
	GetWorld()->GetTimerManager().SetTimer(DotDamageHandle, this, &ACEnemy::DealDotDamage, 1.0f, true);
}

void ACEnemy::ApplyIceEffect(float SlowDuration)
{
	GetCharacterMovement()->MaxWalkSpeed *= 0.5f; // 50% 감소
	GetWorld()->GetTimerManager().SetTimer(SlowHandle, this, &ACEnemy::RemoveIceEffect, SlowDuration, false);
}

void ACEnemy::PlayHitEffect(UNiagaraSystem * Effect, float Duration)
{
	if (Effect)
	{
		UNiagaraComponent* HitEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
			Effect,
			GetMesh(),
			FName("pelvis"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);

		if (HitEffect)
		{
			HitEffect->SetAutoDestroy(true);

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				FTimerDelegate::CreateLambda([HitEffect]()
					{
						HitEffect->Deactivate();
					}),
				Duration,
						false
						);
		}
	}
}

void ACEnemy::DealDotDamage()
{
	if (RemainingDotTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(DotDamageHandle);
		return;
	}

	Status->Damage(TotalDotDamage / RemainingDotTime);
	RemainingDotTime--;

	if (Status->IsDead())
	{
		State->SetDeadMode();
		GetWorld()->GetTimerManager().ClearTimer(DotDamageHandle);
	}
}

void ACEnemy::RemoveIceEffect()
{
	GetCharacterMovement()->MaxWalkSpeed /= 0.5f; // 원래 속도로 복구
}

void ACEnemy::Hit()
{


	// Health 
	Status->Damage(Damage.power);
	Damage.power = 0.0f;

	// Color변경
	{
		Change_Color(this, FLinearColor::Red);

		FTimerDelegate timerDelegate;

		timerDelegate.BindUFunction(this, "RestoreColor");
		GetWorld()->GetTimerManager().SetTimer(RestoreColor_TimerHandle, timerDelegate, 0.2f, false);
	}

	{
		if (!!Damage.Event && !!Damage.Event->HitData)
		{
			// Montage
			Damage.Event->HitData->PlayMontage(this);
			// HitStop
			Damage.Event->HitData->PlayHitStop(GetWorld());
			// Sound
			Damage.Event->HitData->PlaySoundWave(this);
			// Effect
			Damage.Event->HitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

			if (!Status->IsDead())
			{
				FVector start = GetActorLocation();
				FVector target = Damage.Character->GetActorLocation();
				FVector direction = target - start;

				direction = direction.GetSafeNormal();
				LaunchCharacter(-direction * Damage.Event->HitData->Launch, false, false);
				SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
			}

			if (Status->IsDead())
			{
				//   PrintLine();
				State->SetDeadMode();
				return;
			}
		}
	}

	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;

}

void ACEnemy::Dead()
{
	//// Dead된 Hit되지 않게 처리
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->GlobalAnimRateScale = 0.f;
	//GetMesh()->SetSimulatePhysics(true);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//
	//FVector start = GetActorLocation();
	//FVector target = Damage.Character->GetActorLocation();
	//FVector direction = target - start;
	//direction.Normalize();
	//
	//GetMesh()->AddForce(-direction * 2500000);
	Montages->PlayDeadMode();
}

// Noti에서 당은 데이터
void ACEnemy::End_Hit()
{
	State->SetIdleMode();
}

void ACEnemy::End_Dead()
{
	Destroy();
}

void ACEnemy::StealthDead()
{
	PlayAnimMontage(StealthDeadMontage, 1.0, NAME_None);
}
