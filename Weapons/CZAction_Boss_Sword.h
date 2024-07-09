// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CZAction.h"
#include "CZAction_Boss_Sword.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCZAction_Boss_Sword : public UCZAction
{
	GENERATED_BODY()
	
public:
	UCZAction_Boss_Sword();
	virtual void Pressed()    override;
	virtual void Begin_ZAction_Implementation() override;
	virtual void End_ZAction_Implementation() override;
private:
	UPROPERTY(EditAnywhere, Category = "ZoneClass")
	TSubclassOf<class ACZone> ZoneClass;
	UPROPERTY(EditDefaultsOnly, Category = "ZoneClass")
	FVector ZoneLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FDoActionData ActionData;

private:
	class UDecalComponent* Decal;
	
};
