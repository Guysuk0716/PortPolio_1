// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CEnemy_Boss.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CStatusComponent.h"
#include "Items/ItemBase.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Widgets/CUserWidget_Stamina.h"
#include "World/Pickup.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ACEnemy_Boss::ACEnemy_Boss()
{
	PrimaryActorTick.bCanEverTick = true;

	{
		CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
		CHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");
	}

	{
		static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/Enemies/Boss/DT_BossAttackMontage.DT_BossAttackMontage'"));
		if (DataTable.Succeeded())
		{
			AttackMontageDataTable = DataTable.Object;
		}
	}




	bBossRageMode = false;
}

void ACEnemy_Boss::BeginPlay()
{
	Super::BeginPlay();

	UClass* WidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/Widgets/WB_Boss.WB_Boss_C'"));
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

	if (Widget)
	{
		Widget->AddToViewport();

		HPWidget = Cast<UCUserWidget_Stamina>(Widget);
	}

	if (HPWidget)
	{
		UpdateHealthBoss(Status->GetHealth(), Status->GetMaxHealth());
	}

	PlayAnimMontage(StartMontage,0.5f,NAME_None);
}

void ACEnemy_Boss::UpdateHealthBoss(float CurrentHealth, float MaxHealth)
{
	// HPWidget�� ��ȿ���� Ȯ��
	if (HPWidget)
	{
		// Health �� Stamina ������Ʈ �Լ� ȣ��
		HPWidget->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	}
}

void ACEnemy_Boss::PlayTransformCinematic()
{
	if (BossTransformSequence)
	{
		FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* SequenceActor;
		ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(),
			BossTransformSequence,
			Settings,
			SequenceActor
		);

		if (SequencePlayer)
		{
			SequencePlayer->Play();
		}
	}
}

void ACEnemy_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HPWidget)
	{
		UpdateHealthBoss(Status->GetHealth(), Status->GetMaxHealth());
	}

	if (Status->GetHealth() / Status->GetMaxHealth() <= 0.5f)
	{
		if (!bHasAura)
		{
			// Aura를 추가하는 함수 호출
			//PlayTransformCinematic();
			PlayAnimMontage(PhaseMontage, 0.5f, NAME_None);
			AddAura();
			bHasAura = true;
			bBossRageMode = true;
		}
	}

	
}

float ACEnemy_Boss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
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

void ACEnemy_Boss::Hit()
{
	CheckTrue(State->IsDeadMode());
	
	Behavior->SetHittedMode();
	
	// Health 
	Status->Damage(Damage.power);
	Damage.power = 0.0f;

	

	{
		if (!!Damage.Event && !!Damage.Event->HitData)
		{
			// Montage
			Damage.Event->HitData->PlayMontageBoss(this);
			// HitStop
			Damage.Event->HitData->PlayHitStop(GetWorld());
			// Sound
			Damage.Event->HitData->PlaySoundWave(this);
			// Effect
			Damage.Event->HitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

			/*if (!Status->IsDead())
			{
				FVector start = GetActorLocation();
				FVector target = Damage.Character->GetActorLocation();
				FVector direction = target - start;

				direction = direction.GetSafeNormal();
				LaunchCharacter(-direction * Damage.Event->HitData->Launch, false, false);
				SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
			}*/

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
void ACEnemy_Boss::End_Hit()
{
	Super::End_Hit();
	State->SetIdleMode();
	Behavior->SetWaitMode();


}
void ACEnemy_Boss::Dead()
{
	Super::Dead();
	Montages->PlayDeadMode();
	Behavior->SetDeadMode();

}
void ACEnemy_Boss::End_Dead()
{
	Super::End_Dead();
	Destroy();
	Weapon->DestoryWeapon();
}



void ACEnemy_Boss::AddAura()
{
	if (AuraNiagaraSystem)
	{
		UNiagaraComponent* Aura = UNiagaraFunctionLibrary::SpawnSystemAttached(
		   AuraNiagaraSystem, 
		   GetMesh(),         
		   NAME_None,          
		   FVector::ZeroVector,
		   FRotator::ZeroRotator, 
		   EAttachLocation::KeepRelativeOffset, 
		   true              
		);

		if (Aura)
		{
			Aura->SetRelativeLocation(FVector::ZeroVector);
			Aura->SetRelativeRotation(FRotator::ZeroRotator);
		}
	}
}

bool ACEnemy_Boss::IsInBossRageMode() const
{
	return bBossRageMode;
}
