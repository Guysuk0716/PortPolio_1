// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ZActions/CZAction_Fist.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/CGhostTrail.h"


void UCZAction_Fist::Pressed()
{

	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsZActionMode());

	Super::Pressed();

	State->SetActionMode();
	State->OnZActionMode();

	if (!!GhostTrailClass)
		GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);


	ActionData.DoAction(Owner);

}
void UCZAction_Fist::Begin_ZAction_Implementation()
{
	Super::Begin_ZAction_Implementation();

	// ���� DoAction�� Binding�� �� ����
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");

	// FistSubAction Collision Check
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCZAction_Fist::OnAttachmentBeginOverlap);
	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCZAction_Fist::OnAttachmentEndCollision);



}
void UCZAction_Fist::End_ZAction_Implementation()
{
	Super::End_ZAction_Implementation();

	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");

	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);


	// state
	State->SetIdleMode();
	State->OffZActionMode();
	// movement
	Movement->Move();
	Movement->DisableFixedCamera();

	if (!!GhostTrail)
		GhostTrail->Destroy();
	HitIndex = 0;


}

void UCZAction_Fist::OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther)
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


	// ���� �������� ���� �ڵ� �߰�
	if (InOther->IsA(ACharacter::StaticClass()))
	{
		ACharacter* EnemyCharacter = Cast<ACharacter>(InOther);
		if (EnemyCharacter)
		{
			// ���� �������� ���� �ڵ� �߰�
			FVector LaunchDirection = EnemyCharacter->GetActorUpVector();
			LaunchDirection.Normalize();
			float LaunchForce = 1100.0f;
			EnemyCharacter->LaunchCharacter(LaunchDirection * LaunchForce, true, true);

			if (InAttacker)
			{
				InAttacker->LaunchCharacter(LaunchDirection * LaunchForce, true, true);
			}
		}
	}

}

void UCZAction_Fist::OnAttachmentEndCollision()
{
	Hit.Empty();
	HitIndex++;  // 5�� Max
}