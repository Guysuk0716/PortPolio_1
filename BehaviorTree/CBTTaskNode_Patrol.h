// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Patrol.generated.h"

UCLASS()
class U06_BATTLE_API UCBTTaskNode_Patrol : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Patrol();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	UPROPERTY(EditAnywhere, Category = "Patrol")
		bool bDebugMode = true;
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float AcceptanceDistance = 100;
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float RandomRadius = 500;

private:
	void  DrawDebug(UWorld* InWorld, FVector InLocation);

};
