// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"

//***********************************************************
// ����BP������ Weapons : TSubofclass
//                       Weapon->Weapon_Combo->Combo_Sword
// WeaponComponent
//   |-- WeaponAsset
//         |-- Attachment(Actor)  : BPȭ Mesh�� Collider
//         |-- Equipment(Object)  : �������� 
//         |-- DoAction           : Combo
//         |-- SubDoAction        : ���콺Ŭ��
//***********************************************************
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Fist,Sword,Hammer,Warp,Around,Bow,Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U06_BATTLE_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
private:
   UPROPERTY(EditAnywhere, Category = "DataAsset")
        class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

public:
    UFUNCTION(BlueprintCallable)
        void SubAction_Pressed();
    UFUNCTION(BlueprintCallable)
        void SubAction_Released();

public:
	UFUNCTION(BlueprintCallable)
		void ZAction_Pressed();
	UFUNCTION(BlueprintCallable)
		void ZAction_Released();

public:
	UFUNCTION(BlueprintCallable)
		void XAction_Pressed();
	UFUNCTION(BlueprintCallable)
		void XAction_Released();

public:
    void SetUnarmedMode();
    void SetFistMode();
    void SetSwordMode();
    void SetHammerMode();
    void SetWarpMode();
    void SetAroundMode();
    void SetBowMode();

    void DoAction();

public:
	void DestoryWeapon();

private:
    bool IsIdleMode();
    void SetMode(EWeaponType InType);
    void ChangeType(EWeaponType InType);

public:
    class ACAttachment* GetAttachment();
    class UCEquipment*  GetEquipment();
    class UCDoAction*   GetDoAction();
    class UCSubAction*  GetSubAction();
	class UCZAction*    GetZAction();
	class UCXAction*    GetXAction();


private:
    class ACharacter* OwnerCharacter;          // �� ������Ʈ���� ������ ������ ����
    EWeaponType  Type = EWeaponType::Max;


public:
    FWeaponTypeChanged  OnWeaponTypeChanged;   // ȣ�� : ABP,Player,Enemy
private:
    UPROPERTY()  // WeaponAsset���� ������ Ŭ������ ���⿡ ����
        class UCWeaponData* Datas[(int32)EWeaponType::Max];


public:
    FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
    FORCEINLINE bool IsFistMode()    { return Type == EWeaponType::Fist; }
    FORCEINLINE bool IsSwordMode()   { return Type == EWeaponType::Sword; }
    FORCEINLINE bool IsHammerMode()  { return Type == EWeaponType::Hammer; }
    FORCEINLINE bool IsWarpMode()    { return Type == EWeaponType::Warp; }
    FORCEINLINE bool IsAroundMode()  { return Type == EWeaponType::Around; }
    FORCEINLINE bool IsBowMode()     { return Type == EWeaponType::Bow; }

public:
	FORCEINLINE EWeaponType GetWeaponType() { return Type; }







};

