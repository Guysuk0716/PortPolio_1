// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CBossEntranceTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API ACBossEntranceTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	ACBossEntranceTriggerBox();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	bool DoesPlayerHaveRequiredItem(AActor* PlayerActor);

private:
	UPROPERTY(EditAnywhere)
	FName LevelNameToLoad = "Boss_Stage";
	
};
