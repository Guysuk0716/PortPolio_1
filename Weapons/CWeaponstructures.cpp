#include "Weapons/CWeaponstructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Animation/AnimMontage.h"

void FDoActionData::BossAttackMontage(ACharacter* InOwner)
{
    CheckNull(Montage);
    if(!!InOwner)
        InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FDoActionData::DoAction(ACharacter * InOwner)
{
    UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
    if (!!movement)
    {
        if (this->bFixedCamera)
            movement->EnableFixedCamera();
        if (this->bCanMove == false)
            movement->Stop();

    }
    if (!!this->Montage)
        InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FHitData::SendDamage(ACharacter * InAttacker, AActor * InAttackCauser, ACharacter * InOther)
{
    FActionDamageEvent e;
    e.HitData = this;

    InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);

}

void FHitData::PlayMontage(ACharacter * InOwner)
{
    CheckNull(Montage);
    if(!!InOwner)
        InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FHitData::PlayMontageBoss(ACharacter * InOwner)
{
    CheckNull(MontageBoss);
    if(!!InOwner)
        InOwner->PlayAnimMontage(MontageBoss, PlayRate);
}

void FHitData::PlayHitStop(UWorld * InWorld)
{
    // stop time�� 0.0f�ΰ��� return;
    StopPawns.Empty();

    CheckTrue(FMath::IsNearlyZero(StopTime));

    // ���� Level���� ��� ã�´�
    for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
    {
        APawn* pawn = Cast<ACharacter>(actor);
        if (!!pawn)
        {
            pawn->CustomTimeDilation = 1e-3f; // 0.001f
            StopPawns.Add(pawn);
        }
    }

    FTimerDelegate timerDeleagte;

    // BindUFunction��ſ� BindLambda�� �̿�
    // ������ Ŭ�������� ������ �ʰ� ��ȿ������������ ����ρٴ�

    timerDeleagte.BindLambda([=]()
        {
            for (APawn* pawn : StopPawns)
            {
                pawn->CustomTimeDilation = 1;

            }

        });

    FTimerHandle  handle;
    InWorld->GetTimerManager().SetTimer(handle, timerDeleagte, StopTime, false);
}
void FHitData::PlaySoundWave(class ACharacter* InOwner)
{
    CheckNull(Sound);
    CheckNull(InOwner);

    UWorld* world = InOwner->GetWorld();
    FVector location = InOwner->GetActorLocation();
    UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);


}
void FHitData::PlayEffect(UWorld * InWorld, const FVector & InLocation)
{
    //TODO


}

void FHitData::PlayEffect(UWorld * InWorld, const FVector & InLocation, const FRotator & InRotation)
{
    CheckNull(Effect);
    FTransform transform;
    transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));  // FVector,FRotator

    CHelpers::PlayEffect(InWorld, Effect, transform);
}
