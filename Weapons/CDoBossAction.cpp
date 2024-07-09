#include "Weapons/CDoBossAction.h"
#include "Global.h"
#include "Components/CMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Engine/Engine.h"

void UCDoBossAction::DoAction()
{
    if (this->DoActionDatas.Num() < 1)
        return;

    if (bEnable)  // Combo Notity_State����  bEnable�� True
    {
        bEnable = false;
        bExist = true;
        return;
    }

    CheckFalse(State->IsIdleMode());
    Super::DoAction();

    // 확률에따른 랜덤공격
        int32 RandomIndex = FMath::RandRange(0, 100);
    
    if (RandomIndex >= 50)
    {
        DoActionDatas[0].DoAction(OwnerCharacter);
        CLog::Log("DoActionDatas[0]");
        UE_LOG(LogTemp, Warning, TEXT("Selected Random Index: %d"), RandomIndex);
    }
    else if (RandomIndex < 50)
    {
        DoActionDatas[1].DoAction(OwnerCharacter);
        CLog::Log("DoActionDatas[1]");
        UE_LOG(LogTemp, Warning, TEXT("Selected Random Index: %d"), RandomIndex);
    }
   //else if (RandomIndex >= 0)
   //{
   //    DoActionDatas[2].DoAction(OwnerCharacter);
   //    CLog::Log("DoActionDatas[2]");
   //    UE_LOG(LogTemp, Warning, TEXT("Selected Random Index: %d"), RandomIndex);
   //}
    
   

  
  
    
   // int32 RandomIndex = FMath::RandRange(0, DoActionDatas.Num() - 1);
   // DoActionDatas[RandomIndex].DoAction(OwnerCharacter);

}
void UCDoBossAction::Begin_DoAction()
{
    Super::Begin_DoAction();  // bBeginActionó��
    CheckFalse(bExist);

    bExist = false;

    // 확률에따른 랜덤공격
    
    
    int32 RandomIndex = FMath::RandRange(0, 100);
    
    if (RandomIndex >= 50)
    {
        DoActionDatas[0].DoAction(OwnerCharacter);
        CLog::Log("DoActionDatas[0]");
        UE_LOG(LogTemp, Warning, TEXT("Selected Random Index: %d"), RandomIndex);
    }
    else if (RandomIndex >= 20)
    {
        DoActionDatas[1].DoAction(OwnerCharacter);
        CLog::Log("DoActionDatas[1]");
        UE_LOG(LogTemp, Warning, TEXT("Selected Random Index: %d"), RandomIndex);
    }
    else if (RandomIndex >= 0)
    {
        DoActionDatas[2].DoAction(OwnerCharacter);
        CLog::Log("DoActionDatas[2]");
        UE_LOG(LogTemp, Warning, TEXT("Selected Random Index: %d"), RandomIndex);
    }
    
    
   //int32 RandomIndex = FMath::RandRange(0, DoActionDatas.Num() - 1);
   //DoActionDatas[RandomIndex].DoAction(OwnerCharacter);

}

void UCDoBossAction::End_DoAction()
{
    Super::End_DoAction();
}

void UCDoBossAction::OnAttachmentBeginOverlap(ACharacter * InAttacker, AActor * InAttackerCauser, ACharacter * InOther)
{
    Super::OnAttachmentBeginOverlap(InAttacker, InAttackerCauser, InOther);
    CheckNull(InOther);

    // �⺻��
    //InOther->TakeDamage(20.0f, FDamageEvent(), InAttacker->GetController(), InAttackerCauser);

    // ���������� Hit�Ǵ°��� ����
    for (ACharacter* hit : Hit)
        CheckTrue(hit == InOther);

    Hit.AddUnique(InOther);
    CheckTrue(HitDatas.Num() - 1 < Index);
    HitDatas[Index].SendDamage(InAttacker, InAttackerCauser, InOther);

}



void UCDoBossAction::OnAttachmentEndCollision()
{
    Super::OnAttachmentEndCollision();

    // Player�� Hit�Ǹ� �ٶ󺸰� �ϱ�

    float angle = -2.0f;   // dot -1 ~ 1;
    ACharacter* candidate = nullptr;
    for (ACharacter* hit : Hit)
    {
        FVector direction = hit->GetActorLocation() - OwnerCharacter->GetTargetLocation();
        direction = direction.GetSafeNormal2D();   // NormaalLize BP���� tolerence

        // FRoator -> FQuat ( Matrix)
        // m11 m12 m13 m14     ---> RightVector    11,12,13
        // m21 m22 m23 m24     ---> UpVector       21,22,23
        // m31 m32 m33 m34     ---> fowardVector   31,32,33
        // m41 m42 m43 m44
        FVector foward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();

        float dot = FVector::DotProduct(direction, foward);
        if (dot >= angle)
        {
            angle = dot;
            candidate = hit;
        }

    }

    if (!!candidate)
    {
        FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());
        FRotator target = FRotator(0, rotator.Yaw, 0);
        AController* controller = OwnerCharacter->GetController<AController>();
 //       controller->SetControlRotation(target);
    }



    Hit.Empty();


}
