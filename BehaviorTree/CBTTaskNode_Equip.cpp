// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/CBTTaskNode_Equip.h"
#include "Global.h"
#include "CPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Weapons/CEquipment.h"



UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";
	bNotifyTick = true;
}
EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	// 이미장착되어 있다면
	if (Type == weapon->GetWeaponType())
		return EBTNodeResult::Succeeded;

	switch (Type)
	{
	case EWeaponType::Sword:  weapon->SetSwordMode(); break;
	case EWeaponType::Hammer:  weapon->SetSwordMode(); break;
	case EWeaponType::Around:  weapon->SetSwordMode(); break;
	case EWeaponType::Fist:  weapon->SetSwordMode(); break;
	case EWeaponType::Warp:   weapon->SetWarpMode();  break;
	case EWeaponType::Bow:    weapon->SetBowMode();   break;
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	weapon->GetEquipment()->End_Equip();
	return EBTNodeResult::Aborted;
}


void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent*  state = CHelpers::GetComponent<UCStateComponent>(ai);

	const bool* bEquipped = weapon->GetEquipment()->GetEquipped();
	// montage run되고 begin_equip, end_equip이 완료되면
	// idle상태
	if (state->IsIdleMode() && *bEquipped)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

}
