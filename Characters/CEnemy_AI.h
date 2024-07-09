// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "World/Pickup.h"
#include "Items/ItemBase.h"
#include "CEnemy_AI.generated.h"



UCLASS()
class U06_BATTLE_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()
public:
	ACEnemy_AI();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;      //BT_Melee,BT_Range
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8  TeamID = 2;                     // 255 == �߸�, TeamID�� �����ϸ� �Ʊ�
	UPROPERTY(EditAnywhere, Category = "Patrol")
		class ACPatrolPath* PatrolPath;
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponComponent* Weapon;
	UPROPERTY(VisibleDefaultsOnly)
		class UCAIBehaviorComponent* Behavior;


protected:
	void DropItems();

	UPROPERTY(EditAnywhere, Category = "Drop")
	UDataTable* ItemDataTable; 
	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<APickup> DropItemClass;
	UPROPERTY(EditAnywhere, Category = "Drop")
	FName DesiredItemID; 
	UPROPERTY(EditAnywhere, Category = "Drop")
	int32 DropItemQuantity;
		


private:
	UPROPERTY(EditDefaultsOnly, Category = "Label")
		float  LabelViewAmount = 3000.0f;   // �����Ÿ������� UI�� ���ϼ� �ְ�

#if WITH_EDITOR  // GameMode������ ������� �ʰ�
private:
	UPROPERTY(EditDefaultsOnly, Category = "Label")
		class  UWidgetComponent*  LabelWidget;
#endif

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
	void UpdateLabelRenderScale();
	
};
