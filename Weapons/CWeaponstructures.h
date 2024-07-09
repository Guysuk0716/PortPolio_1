// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Components/CStateComponent.h"
#include "CWeaponstructures.generated.h"

// BP에서 사용된 Structure를 직렬화 여기에 구성
// 장비장착에 관련 데이터 구조체
// 구조체를 UE-C에서 구현할때는 전부 EditAnywhere
USTRUCT(BlueprintType)
struct FHitMontageData : public FTableRowBase
{
public:
    GENERATED_BODY()
    UPROPERTY(EditAnywhere)
    EHitReactionType HitType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UAnimMontage* Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayRate;
};

USTRUCT(BlueprintType)
struct FBossAttackMontageData : public FTableRowBase
{
public:
    GENERATED_BODY()
    UPROPERTY(EditAnywhere)
    EBossAttackType BossAttackType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UAnimMontage* Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayRate;
};
USTRUCT()
struct FEquipmentData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
        class UAnimMontage* Montage;
    UPROPERTY(EditAnywhere)
        float PlayRate = 1.0f;
    UPROPERTY(EditAnywhere)
        bool  bCanMove = true;
    UPROPERTY(EditAnywhere)
        bool  bUseControlRotation = true;

};
// DoAction에 관련된 데이터
USTRUCT()
struct FDoActionData
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
    class UAnimMontage* Montage;
    
public:
    void BossAttackMontage(ACharacter* InOwner);
    

    
    UPROPERTY(EditAnywhere)
        float PlayRate = 1.0f;
    UPROPERTY(EditAnywhere)
        bool  bCanMove = true;
    UPROPERTY(EditAnywhere)
        bool  bFixedCamera;
    // 파티클 , 나이아가라
    UPROPERTY(EditAnywhere)
        class UFXSystemAsset* Effect; 
    UPROPERTY(EditAnywhere)
        FVector EffectLocation = FVector::ZeroVector;
    UPROPERTY(EditAnywhere)
        FVector EffectScale   = FVector::OneVector;
public:
    void  DoAction(class ACharacter* InOwner);
};
// Hit에 관련된 데이터
USTRUCT()
struct FHitData
{
    GENERATED_BODY()
public:
    
    UPROPERTY(EditAnywhere)
    class UAnimMontage* Montage;
    
    UPROPERTY(EditAnywhere)
    class UAnimMontage* MontageBoss;
    UPROPERTY(EditAnywhere)
        float PlayRate = 1.0f;
    UPROPERTY(EditAnywhere)   
        float Power = 0.0f;         // Damage
    UPROPERTY(EditAnywhere)
        float Launch = 100.0f;
    UPROPERTY(EditAnywhere)
        float StopTime = 0.0f;    // Hit Stop
    UPROPERTY(EditAnywhere)
        class USoundWave* Sound;

    UPROPERTY(EditAnywhere)
        class UFXSystemAsset* Effect;
    UPROPERTY(EditAnywhere)
        FVector EffectLocation = FVector::ZeroVector;
    UPROPERTY(EditAnywhere)
        FVector EffectScale = FVector::OneVector;

public:
    void SendDamage(class ACharacter*  InAttacker, AActor* InAttackCauser, class ACharacter* InOther);
    void PlayMontage(class ACharacter* InOwner);
    void PlayMontageBoss(class ACharacter* InOwner);
    void PlayHitStop(UWorld* InWorld);
    void PlaySoundWave(class ACharacter* InOwner);
    void PlayEffect(UWorld* InWorld, const FVector& InLocation);
    void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);

public:
    TArray<APawn*>  StopPawns;



};
USTRUCT()
struct FActionDamageEvent : public FDamageEvent
{
    GENERATED_BODY()
public:
    FHitData* HitData;
};

// USTRUCT()
// struct FDamageData
// {
//     float  power = 0.0f;
//     class  ACharacter* Character;
//     class  AActor*     Causer;
//     struct FActionDamageEvent* Event;
// };



UCLASS()
class U06_BATTLE_API UCWeaponstructures : public UObject
{
	GENERATED_BODY()
	
};
