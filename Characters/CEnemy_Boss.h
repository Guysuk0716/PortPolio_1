// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "ICharacter.h"
#include "NiagaraSystem.h"
#include "Components/CStateComponent.h"
#include "Engine/DataTable.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Widgets/CUserWidget_Stamina.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponstructures.h"
#include "CEnemy_Boss.generated.h"

/**
 * 
 */


UCLASS()
class U06_BATTLE_API ACEnemy_Boss : public ACEnemy
{
	GENERATED_BODY()

public:
	ACEnemy_Boss();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;      //BT_Melee,BT_Range
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	uint8  TeamID = 2;                     // 255 == 중립, TeamID가 동일하면 아군
	UPROPERTY(EditAnywhere, Category = "AI")
	class ACPatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, Category = "StartMontage")
	UAnimMontage* StartMontage;

	UPROPERTY(EditAnywhere, Category = "PhaseMontage")
		UAnimMontage* PhaseMontage;

	

private:
	UPROPERTY(VisibleDefaultsOnly)
	class UCWeaponComponent* Weapon;
	UPROPERTY(VisibleDefaultsOnly)
	class UCAIBehaviorComponent* Behavior;
	
public:   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
	UDataTable* AttackMontageDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
		ULevelSequence* BossTransformSequence;
public:
	// AActor::TakeDamage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator, AActor * DamageCauser) override;



public:
	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }

public:   // Enmey-->EnemyAI
	void Hit()  override;
	void Dead() override;

public:   // Interface���� ��� ���� �Լ� oeveride
	void End_Hit()   override;
	void End_Dead()  override;


	
private:
	bool bBossRageMode;
	bool bHasAura; 
	void AddAura();
	
	UPROPERTY(EditAnywhere, Category = "2Page BodyAura")
	UNiagaraSystem* AuraNiagaraSystem;
	
public:
	bool IsInBossRageMode() const;

private:
	// CUserWidget_Stamina Ŭ������ ������ ����
	UCUserWidget_Stamina* HPWidget;
public:
	void UpdateHealthBoss(float CurrentHealth, float MaxHealth);

	void PlayTransformCinematic();

};