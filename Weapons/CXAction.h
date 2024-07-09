// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponstructures.h"
#include "CXAction.generated.h"

//Blueprintable
//UCLASS(Abstract)
UCLASS(Blueprintable)
class U06_BATTLE_API UCXAction : public UObject
{
	GENERATED_BODY()

public:
	UCXAction();
public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);

public: // ���콺
	virtual void Pressed();
	virtual void Released();

	// �������� �ۼ��� �͵� --> ��� ������ ����
	/*
	BlueprintCallable
	--> C++ �� �ۼ���.�������Ʈ �׷������� ȣ�� ���������� �����̳� ������ �Ұ���(e.g.���� �Լ�)

	BlueprintImplementableEvent
	--> ��� ����(.h) �� �߰�������, �Լ� ������ C++ �� �ƴ� �������Ʈ �׷������� �ۼ���.
		ǥ�� ������ ���� �̺�Ʈ�� �����Ӱ� ������ �� �ֵ��� �ϴ� ��쿡 ���. (e.g.�� �������� �ٸ� ������ ��)

	BlueprintNativeEvent
	--> BlueprintCallable �� BlueprintImplementableEvent �� ������.C++ �� �ۼ��Ǿ�����, �������Ʈ���� ���� �Ǵ� ��ü ������.
		BlueprintNativeEvent �� ����� ���� �Լ� �������� "_Implementation" �� �ٿ��� ��.
	*/


public:
	UFUNCTION(BlueprintNativeEvent)
		void Begin_XAction();
	virtual void Begin_XAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void End_XAction();
	virtual void End_XAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {};

protected:
	bool    bInAction = false; // action������ check flag
	UPROPERTY()
		class   ACharacter*         Owner;
	UPROPERTY()
		class   ACAttachment*      Attachment;
	UPROPERTY()
		class   UCDoAction*        DoAction;
	UPROPERTY()
		class   UCStateComponent*        State;
	UPROPERTY()
		class   UCMovementComponent*         Movement;

public:
	FORCEINLINE bool GetInAction() { return bInAction; }
};
