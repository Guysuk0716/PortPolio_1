// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CWeaponComponent.h"
#include "CBTTaskNode_Equip_Boss.generated.h"


UCLASS()
class U06_BATTLE_API UCBTTaskNode_Equip_Boss : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Equip_Boss();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	UPROPERTY(EditAnywhere, Category = "Type")
	EWeaponType Type;


};