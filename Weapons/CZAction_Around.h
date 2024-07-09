// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CZAction.h"
#include "Weapons/CWeaponstructures.h"
#include "CZAction_Around.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCZAction_Around : public UCZAction
{
	GENERATED_BODY()
	
public:
	UCZAction_Around();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Object")
		TSubclassOf<class ACBlackHole> ObjectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Object")
		FVector ObjectLocation;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;


public: // ¸¶¿ì½º
	virtual void Pressed()    override;
	virtual void Begin_ZAction_Implementation() override;
	virtual void End_ZAction_Implementation() override;
};
