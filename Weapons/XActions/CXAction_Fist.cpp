// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/XActions/CXAction_Fist.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/CGhostTrail.h"


void UCXAction_Fist::Pressed()
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
void UCXAction_Fist::Begin_XAction_Implementation()
{
	Super::Begin_XAction_Implementation();

	// ���� DoAction�� Binding�� �� ����
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");

	// FistSubAction Collision Check
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCXAction_Fist::OnAttachmentBeginOverlap);
	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCXAction_Fist::OnAttachmentEndCollision);



}
void UCXAction_Fist::End_XAction_Implementation()
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

void UCXAction_Fist::OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther)
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


	// ���� �ٴ����� �ڵ� �߰�
	if (InOther->IsA(ACharacter::StaticClass()))
	{
		ACharacter* EnemyCharacter = Cast<ACharacter>(InOther);
		if (EnemyCharacter)
		{
			// ���� �ٴ����� �ڵ� �߰�
			FVector DownwardDirection = FVector::DownVector;
			DownwardDirection.Normalize();
			float DownwardForce = 5000.0f;
			EnemyCharacter->LaunchCharacter(DownwardDirection * DownwardForce, true, true);

			//  ����ĳ���͵� ���� ��������� ���
			//if (InAttacker)
			//{
			//	InAttacker->LaunchCharacter(LaunchDirection * DownwardForce, true, true);
			//}
		}
	}

}

void UCXAction_Fist::OnAttachmentEndCollision()
{
	Hit.Empty();
	HitIndex++;  // 5�� Max
}