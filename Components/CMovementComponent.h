// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovementComponent.generated.h"

UENUM()
enum class ESpeedType : uint8
{
    Walk=0, Run, Sprint,Max,Dash
};

// BP���� ������Ʈ�� �߰����� �ʰ� �Ϸ���
UCLASS(ClassGroup = (Custom))
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U06_BATTLE_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMovementComponent();
protected:
	virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "CameraSpeed")
        float HorizontalLook = 45.0f;
    UPROPERTY(EditAnywhere, Category = "CameraSpeed")
        float VerticalLook = 45.0f;

private:
    UPROPERTY(EditAnywhere, Category = "Speed")
        float Speed[(int32)ESpeedType::Max] = { 200,400,600 };

public:
    FORCEINLINE bool CanMove() { return bCanMove; }
    FORCEINLINE void Move() { bCanMove = true; }
    FORCEINLINE void Stop() { bCanMove = false; }

    FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ESpeedType::Walk]; }
    FORCEINLINE float GetRunSpeed() { return Speed[(int32)ESpeedType::Run]; }
    FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ESpeedType::Sprint]; }

    FORCEINLINE bool GetFixedCamera() { return bFixedCamera; }
    FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
    FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

    FORCEINLINE void EnableTopViewCamera()  { bTopViewCamera = true; }
    FORCEINLINE void DisableTopViewCamera() { bTopViewCamera = false; }



public:
    void OnMoveForward(float InAxis);
    void OnMoveRight(float InAxis);
    void OnHorizontalLook(float InAxis);
    void OnVerticalLook(float InAxis);

public:
    void EnableControlRotation();
    void DisableControlRotation();

public:
    void OnWalk();
    void OnRun();
    void OnSprint();
    void SetSpeed(ESpeedType InType);


private:
    bool bCanMove = true;
    bool bFixedCamera = false;
    bool bTopViewCamera = false;

	// �뽬 
public:
	void OnDashing();
	void OffDashing();
	float DashDistance = 3000.0f;
	class ACGhostTrail* GhostTrail;

	UPROPERTY(EditAnywhere, Category = "Add-On")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

	// ���¹̳�
public:
	float MaxStamina; // �ִ� ���¹̳�
	float CurrentStamina; // ���� ���¹̳�
	float DashStaminaCost; // ��� ���ۿ� �Һ�Ǵ� ���¹̳�
	float StaminaRecoveryRate;
	float HammerStaminaCost = 5.0f;
	float AroundStaminaCost = 5.0f;




public:
	void RecoverStamina(float DeltaTime);


private:
	UPROPERTY(EditAnywhere)
		UAnimMontage* DashMontage;

public:
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float GetStamina() { return CurrentStamina; }


private:
    class ACharacter* OwnerCharacter;  // �� ������Ʈ���� ������ ������ ����

public:
	void SetStamina(float InStamina);
};
