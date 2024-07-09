// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/CBTTaskNode_Action.h"
#include "Global.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";
	bNotifyTick = true;
}
EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	controller->StopMovement();
	weapon->DoAction();

	return EBTNodeResult::InProgress;
}
EBTNodeResult::Type UCBTTaskNode_Action::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());
	if (!AI || AI->IsPendingKillPending())
	{
		return EBTNodeResult::Failed;
	}

	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(AI);
	if (!Weapon)
	{
		return EBTNodeResult::Failed;
	}

	// weapon이 유효한 경우에만 코드를 실행합니다.
	bool bBeginAction = Weapon->GetDoAction()->GetBeginDoAction();
	if (!bBeginAction)
	{
		Weapon->GetDoAction()->Begin_DoAction();
	}
	Weapon->GetDoAction()->End_DoAction();

	return EBTNodeResult::Succeeded;
}
void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent*  state = CHelpers::GetComponent<UCStateComponent>(ai);

	bool bCheck = true;

	bCheck &= (state->IsIdleMode());
	bCheck &= (weapon->GetDoAction()->GetInAction() == false);

	if (bCheck)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}