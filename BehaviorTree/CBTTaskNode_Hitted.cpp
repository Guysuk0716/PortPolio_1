// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/CBTTaskNode_Hitted.h"
#include "Global.h"
#include "CPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_Boss.h"
#include "Characters/BossController.h"
#include "NavigationSystem.h"


UCBTTaskNode_Hitted::UCBTTaskNode_Hitted()
{
	NodeName = "Hitted";
	bNotifyTick = true;
}
EBTNodeResult::Type UCBTTaskNode_Hitted::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
		if(controller)
		{
			controller->StopMovement();
		}
	
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Hitted::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(controller->GetPawn());
	
	UCStateComponent*  state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (!state->IsHitMode())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	
}
