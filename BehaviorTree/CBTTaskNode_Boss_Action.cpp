// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/CBTTaskNode_Boss_Action.h"
#include "Global.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_Boss.h"
#include "Characters/BossController.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"

UCBTTaskNode_Boss_Action::UCBTTaskNode_Boss_Action()
{
	NodeName = "Action";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Boss_Action::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABossController* controller = Cast<ABossController>(OwnerComp.GetOwner());
	if (!controller)
	{
		UE_LOG(LogTemp, Error, TEXT("Controller not found"));
		return EBTNodeResult::Failed;
	}

	ACEnemy_Boss* ai = Cast<ACEnemy_Boss>(controller->GetPawn());
	if (!ai)
	{
		UE_LOG(LogTemp, Error, TEXT("AI pawn not found"));
		return EBTNodeResult::Failed;
	}

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	if (!weapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon component not found"));
		return EBTNodeResult::Failed;
	}

	UE_LOG(LogTemp, Log, TEXT("Executing DoAction"));
	controller->StopMovement();
	weapon->DoAction();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_Boss_Action::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ABossController* Controller = Cast<ABossController>(OwnerComp.GetOwner());
	if (!Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("Controller not found in AbortTask"));
		return EBTNodeResult::Failed;
	}

	ACEnemy_Boss* AI = Cast<ACEnemy_Boss>(Controller->GetPawn());
	if (!AI || AI->IsPendingKillPending())
	{
		UE_LOG(LogTemp, Error, TEXT("AI pawn not found or pending kill in AbortTask"));
		return EBTNodeResult::Failed;
	}

	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(AI);
	if (!Weapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon component not found in AbortTask"));
		return EBTNodeResult::Failed;
	}

	UE_LOG(LogTemp, Log, TEXT("Aborting DoAction"));
	if (Weapon->GetDoAction()->GetBeginDoAction())
	{
		Weapon->GetDoAction()->End_DoAction();
	}
	else
	{
		Weapon->GetDoAction()->Begin_DoAction();
		Weapon->GetDoAction()->End_DoAction();
	}

	return EBTNodeResult::Succeeded;
}

void UCBTTaskNode_Boss_Action::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ABossController* controller = Cast<ABossController>(OwnerComp.GetOwner());
	if (!controller)
	{
		UE_LOG(LogTemp, Error, TEXT("Controller not found in TickTask"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ACEnemy_Boss* ai = Cast<ACEnemy_Boss>(controller->GetPawn());
	if (!ai)
	{
		UE_LOG(LogTemp, Error, TEXT("AI pawn not found in TickTask"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	if (!weapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon component not found in TickTask"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (!state)
	{
		UE_LOG(LogTemp, Error, TEXT("State component not found in TickTask"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	bool bIsIdleMode = state->IsIdleMode();
	bool bIsInAction = weapon->GetDoAction()->GetInAction();



	bool bCheck = true;
	bCheck &= bIsIdleMode;
	bCheck &= !bIsInAction;

	if (bCheck)
	{
		UE_LOG(LogTemp, Log, TEXT("Action and state conditions met, finishing task"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}