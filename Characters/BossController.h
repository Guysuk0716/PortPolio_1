// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossController.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API ABossController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABossController();
protected:
	virtual void  BeginPlay()       override;

protected:
	void   OnPossess(APawn* InPawn)  override;
	void   OnUnPossess()             override;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:   // 바인딩
	UFUNCTION()
	void  OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* Perception;


private: // 외변에서 만들어진 데이터 보관
	class ACEnemy_Boss*           Enemy;
	class UAISenseConfig_Sight* Sight;
	class UCAIBehaviorComponent* Behavior;
};

