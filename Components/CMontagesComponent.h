// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Components/CStateComponent.h"
#include "CMontagesComponent.generated.h"

USTRUCT()
struct FMontagesData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		EStateType Type;
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class U06_BATTLE_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCMontagesComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "DataTable")
		UDataTable* DataTable;
private:
	UFUNCTION()
		void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	void PlayBackStepMode();
	void PlayDeadMode();
	void PlayRollMode();
private:
	void PlayAnimMontage(EStateType InType);

private:
	FMontagesData* Datas[(int32)EStateType::Max];  // �����ʹ� ������������ return ����
private:
	class ACharacter* OwnerCharacter;  // �� ������Ʈ���� ������ ������ ����

public:
	void PlayFKeyMontage();
	void PlayBlockAnimation();

private:
	// ����� �ִϸ��̼� ��Ÿ�ָ� ���� UAnimMontage ������
	UAnimMontage* FKeyMontage;
	UAnimMontage* BlockMontage;
};

