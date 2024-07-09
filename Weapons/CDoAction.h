// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponstructures.h"
#include "CDoAction.generated.h"

// CDoaction-->CDoAction_Sword
// class�� �߻�ȭ : virtual�Լ��� �����ϸ� �߻�Ŭ��������Ѵ�
//
//UCLASS(Abstract, NotBlueprintable)
UCLASS()

class U06_BATTLE_API UCDoAction : public UObject
{
	GENERATED_BODY()
	
public:
    UCDoAction();
public: // Actor,AtctorCompont Like�ϰ�
    virtual void BeginPlay
    (
        class ACharacter*   InOwner,
        class ACAttachment* InAttachment,
        class UCEquipment*  InEquipment,
        const TArray<FDoActionData>& InDoActionData, 
        const TArray<FHitData>& InHitData
    );
    virtual void Tick(float  InDeltaTime) {};  // ��ӹ޴� ���� override

public:
    virtual void DoAction();          // �⺻���� �Ұ��� �� Ŭ������ ����
    virtual void Begin_DoAction();
    virtual void End_DoAction();

public:
    UFUNCTION()
        virtual void OnBeginEquip() {};
    UFUNCTION()
        virtual void OnUnEquip() {};


protected:
    UPROPERTY()  // -> UObect �������ݷ��ǿ��� ��������...(UProperty,TArray,61��)
        class ACharacter*          OwnerCharacter;
    UPROPERTY()
        class UCMovementComponent* Movement;
    UPROPERTY()
        class UCStateComponent*    State;
    UPROPERTY()
        class UWorld*              World;

public:  // Collision
    UFUNCTION()
        virtual void OnAttachmentBeginCollision() {};
    UFUNCTION()
        virtual void OnAttachmentEndCollision() {};
    UFUNCTION()
        virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker,AActor* InAttackerCauser, class ACharacter* InOther) {};
    UFUNCTION()
        virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) {};


protected:
    TArray<FDoActionData>  DoActionDatas;
    TArray<FHitData>       HitDatas;

public:
    bool bInAction    = false;
    bool bBeginAction = false;

public:
	FORCEINLINE bool GetBeginDoAction() { return bBeginAction; }
	FORCEINLINE bool GetInAction() { return bInAction; }

};
