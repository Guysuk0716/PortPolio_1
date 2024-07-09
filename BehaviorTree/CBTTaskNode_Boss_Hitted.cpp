// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/CBTTaskNode_Boss_Hitted.h"
#include "Global.h"
#include "CPatrolPath.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_Boss.h"
#include "Characters/BossController.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"


UCBTTaskNode_Boss_Hitted::UCBTTaskNode_Boss_Hitted()
{
	NodeName = "Hitted";
	bNotifyTick = true;
}
EBTNodeResult::Type UCBTTaskNode_Boss_Hitted::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	ABossController* controller = Cast<ABossController>(OwnerComp.GetOwner());
	if(controller)
	{
		controller->StopMovement();
	}
	
	ACEnemy_Boss* ai = Cast<ACEnemy_Boss>(controller->GetPawn());
	

	
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Boss_Hitted::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	ABossController* controller = Cast<ABossController>(OwnerComp.GetOwner());
	ACEnemy_Boss*     ai = Cast<ACEnemy_Boss>(controller->GetPawn());

	


	UCStateComponent*  state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (!state->IsHitMode())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}


	
	
}