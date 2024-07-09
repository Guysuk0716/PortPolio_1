// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/CBTTaskNode_Equip_Boss.h"
#include "Global.h"
#include "CPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_Boss.h"
#include "Characters/BossController.h"
#include "Weapons/CEquipment.h"



UCBTTaskNode_Equip_Boss::UCBTTaskNode_Equip_Boss()
{
	NodeName = "Equip";
	bNotifyTick = true;
}
EBTNodeResult::Type UCBTTaskNode_Equip_Boss::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	ABossController* controller = Cast<ABossController>(OwnerComp.GetOwner());
	ACEnemy_Boss*     ai = Cast<ACEnemy_Boss>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	// �̹������Ǿ� �ִٸ�
	if (Type == weapon->GetWeaponType())
		return EBTNodeResult::Succeeded;

	switch (Type)
	{
	case EWeaponType::Sword:  weapon->SetSwordMode(); break;
	case EWeaponType::Fist:  weapon->SetFistMode(); break;

	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_Equip_Boss::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ABossController* controller = Cast<ABossController>(OwnerComp.GetOwner());
	ACEnemy_Boss*     ai = Cast<ACEnemy_Boss>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	weapon->GetEquipment()->End_Equip();
	return EBTNodeResult::Aborted;
}


void UCBTTaskNode_Equip_Boss::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	ABossController* controller = Cast<ABossController>(OwnerComp.GetOwner());
	ACEnemy_Boss*     ai = Cast<ACEnemy_Boss>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent*  state = CHelpers::GetComponent<UCStateComponent>(ai);

	const bool* bEquipped = weapon->GetEquipment()->GetEquipped();
	// montage run�ǰ� begin_equip, end_equip�� �Ϸ�Ǹ�
	// idle����
	if (state->IsIdleMode() && *bEquipped)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

}
