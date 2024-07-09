// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Boss_Action_wave.generated.h"

/**
 *
 */
UCLASS()
class U06_BATTLE_API UCBTTaskNode_Boss_Action_wave : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Boss_Action_wave();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


};