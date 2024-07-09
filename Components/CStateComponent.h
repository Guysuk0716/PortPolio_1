// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
    Idle, BackStep,Roll,Equip,Hit,Dead,Action,Max,Block
};

UENUM()
enum class EHitReactionType : uint8
{
    Idle = 0, Front,Right,Left,Back,
};

UENUM()
enum class EBossAttackType : uint8
{
    Idle = 0, Melee,Range,Rage
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U06_BATTLE_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
    void SetIdleMode();
    void SetBackStepMode();
    void SetRollMode();
    void SetEquipMode();
    void SetHitMode();
    void SetActionMode();
    void SetDeadMode();
    void SetBlockMode();

public:
    void OnSubActionMode();
    void OffSubActionMode();

public:
	void OnZActionMode();
	void OffZActionMode();


public:
	void OnXActionMode();
	void OffXActionMode();



private:
    void ChangeType(EStateType InType);   // Binding --> ABP,Player,Enemey

public:
    FStateTypeChanged  OnStateTypeChanged;



private:
    EStateType Type = EStateType::Idle;
    EHitReactionType HitType = EHitReactionType::Idle;
    EBossAttackType BossattackType = EBossAttackType::Idle;
private:
    bool    bInSubActionMode = false;
private:
	bool    bInZActionMode = false;
private:
	bool    bInXActionMode = false;

private:
    class ACharacter* OwnerCharacter;  // 각 콤포넌트마다 생성시 변수를 저장

public:
    FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
    FORCEINLINE bool IsBackStepMode() { return Type == EStateType::BackStep; }
    FORCEINLINE bool IsRollMode() { return Type == EStateType::Roll; }
    FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
    FORCEINLINE bool IsHitMode() { return Type == EStateType::Hit; }
    FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
    FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
    FORCEINLINE bool IsBlockMode() { return Type == EStateType::Block; }
  


    FORCEINLINE bool IsSubActionMode() { return bInSubActionMode; }
	FORCEINLINE bool IsZActionMode() { return bInZActionMode; }
	FORCEINLINE bool IsXActionMode() { return bInXActionMode; }
};
