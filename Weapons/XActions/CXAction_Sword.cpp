// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/XActions/CXAction_Sword.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/CGhostTrail.h"
#include "Components/CAIBehaviorComponent.h"


void UCXAction_Sword::Pressed()
{

	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsXActionMode());

	Super::Pressed();

	State->SetActionMode();
	State->OnXActionMode();

	if (!!GhostTrailClass)
		GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);


	ActionData.DoAction(Owner);

}

void UCXAction_Sword::Begin_XAction_Implementation()
{
	Super::Begin_XAction_Implementation();

	// 기존 DoAction에 Binding된 것 삭제
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");

	// SwordSubAction Collision Check
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCXAction_Sword::OnAttachmentBeginOverlap);
	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCXAction_Sword::OnAttachmentEndCollision);





}
void UCXAction_Sword::End_XAction_Implementation()
{
	Super::End_XAction_Implementation();

	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");

	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);


	// state
	State->SetIdleMode();
	State->OffXActionMode();
	// movement
	Movement->Move();
	Movement->DisableFixedCamera();

	if (!!GhostTrail)
		GhostTrail->Destroy();
	HitIndex = 0;






}

void UCXAction_Sword::OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther)
{

	CheckNull(InOther);
	// Hit된 Character을 배열에 저장
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

void UCXAction_Sword::OnAttachmentEndCollision()
{
	Hit.Empty();
	HitIndex++;  // 5가 Max


}