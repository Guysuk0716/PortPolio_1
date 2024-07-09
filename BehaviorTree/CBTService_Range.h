// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Range.generated.h"

UCLASS()
class U06_BATTLE_API UCBTService_Range : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Range();
protected:
	// BP���� Recive Tick AI
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
		float  AvoidRange = 500.0f;
	UPROPERTY(EditAnywhere, Category = "Action")
		bool   bDrawDebug = true;

};