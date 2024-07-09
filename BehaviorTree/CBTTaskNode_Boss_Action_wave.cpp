// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/CBTTaskNode_Boss_Action_wave.h"
#include "Global.h"
#include "Components/CAIBehaviorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CEnemy_Boss.h"
#include "Characters/BossController.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"

UCBTTaskNode_Boss_Action_wave::UCBTTaskNode_Boss_Action_wave()
{
	NodeName = "Wave";
	bNotifyTick = true;
}
EBTNodeResult::Type UCBTTaskNode_Boss_Action_wave::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABossController* controller = Cast<ABossController>(OwnerComp.GetOwner());
	ACEnemy_Boss*     ai = Cast<ACEnemy_Boss>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	

	controller->StopMovement();
	weapon->SubAction_Pressed();

	return EBTNodeResult::InProgress;
}
EBTNodeResult::Type UCBTTaskNode_Boss_Action_wave::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ABossController* Controller = Cast<ABossController>(OwnerComp.GetOwner());
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	ACEnemy_Boss* AI = Cast<ACEnemy_Boss>(Controller->GetPawn());
	if (!AI || AI->IsPendingKillPending())
	{
		return EBTNodeResult::Failed;
	}

	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(AI);
	if (!Weapon)
	{
		return EBTNodeResult::Failed;
	}

	// weapon�� ��ȿ�� ��쿡�� �ڵ带 �����մϴ�.
	bool bBeginAction = Weapon->GetDoAction()->GetBeginDoAction();
	if (!bBeginAction)
	{
		Weapon->GetSubAction()->Begin_SubAction();
	}
	Weapon->GetSubAction()->End_SubAction();

	return EBTNodeResult::Succeeded;
}
void UCBTTaskNode_Boss_Action_wave::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	ABossController* controller = Cast<ABossController>(OwnerComp.GetOwner());
	ACEnemy_Boss*     ai = Cast<ACEnemy_Boss>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent*  state = CHelpers::GetComponent<UCStateComponent>(ai);

	bool bCheck = true;

	bCheck &= (state->IsIdleMode());
	bCheck &= (weapon->GetSubAction()->GetInAction() == false);

	if (bCheck)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}