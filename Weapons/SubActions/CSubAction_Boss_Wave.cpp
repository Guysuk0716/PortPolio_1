// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_Boss_Wave.h"
#include "BehaviorTree/BTService_Boss.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"


void UCSubAction_Boss_Wave::Pressed()
{

    CheckFalse(State->IsIdleMode());
    CheckTrue(State->IsSubActionMode());

    Super::Pressed();

    State->SetActionMode();
    State->OnSubActionMode();

    
    ActionData.DoAction(Owner);
    PrintLine();

}
void UCSubAction_Boss_Wave::Begin_SubAction_Implementation()
{
    Super::Begin_SubAction_Implementation();

    // ���� DoAction�� Binding�� �� ����
    Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
    Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");

    // FistSubAction Collision Check
    Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSubAction_Boss_Wave::OnAttachmentBeginOverlap);
    Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSubAction_Boss_Wave::OnAttachmentEndCollision);

    

    PrintLine();

}
void UCSubAction_Boss_Wave::End_SubAction_Implementation()
{
    Super::End_SubAction_Implementation();

    Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
    Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");

    Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
    Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);


    // state
    State->SetIdleMode();
    State->OffSubActionMode();
    // movement
    Movement->Move();
    Movement->DisableFixedCamera();
    
    HitIndex = 0;
    PrintLine();

}

void UCSubAction_Boss_Wave::OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther)
{

    CheckNull(InOther);
    // Hit�� Character�� �迭�� ����
    for (ACharacter* character : Hit)
    {
        if (InOther == character)
            return;

    }

    Hit.AddUnique(InOther);

    // HitIndex Check
    CheckTrue(HitIndex >= HitDatas.Num());
    HitDatas[HitIndex].SendDamage(Owner, InAttackCuaser, InOther);

}

void UCSubAction_Boss_Wave::OnAttachmentEndCollision()
{
    Hit.Empty();
    HitIndex++;  // 5�� Max
}