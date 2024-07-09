// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPlayerController.h"
#include "GameFramework/Character.h"
#include "ICharacter.h"
#include "Components/CStateComponent.h"
#include "Components/CParkourComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Widgets/CUserWidget_Stamina.h"
#include "Interfaces/InteractionInterface.h"
#include "Components/InventoryComponent.h"
#include "CPlayer.generated.h"

class ACSTutorialHUD;
class UCWeaponComponent;


USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

		FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	}

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

class UWidgetComponent;


UCLASS()
class U06_BATTLE_API ACPlayer
	: public ACharacter
	, public IICharacter
	, public IGenericTeamAgentInterface
	
{
	GENERATED_BODY()

public:
	ACPlayer();

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	FORCEINLINE UInventoryComponent* GetInventory() const {return PlayerInventory;};

	void UpdateInteractionWidget() const;

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void  Landed(const FHitResult& Hit) override;
public:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override { return  TeamID; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		FGenericTeamId TeamID = FGenericTeamId(1);
private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent*    Camera;

public:
	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;
	UPROPERTY(VisibleAnywhere)
		class UCStateComponent*    State;
	UPROPERTY(VisibleAnywhere)
		class UCMontagesComponent*  Montages;
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent*    Weapon;
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType PrevType, EWeaponType NewType);
	UPROPERTY(VisibleAnywhere)
		class UCParkourComponent*   Parkour;
	UPROPERTY(VisibleAnywhere)
		class UCFeetComponent*      Feet;
	UPROPERTY(VisibleAnywhere)
		class UCZoomComponent*      Zoom;
	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent*     Status;
	UPROPERTY(EditAnywhere, Category = "Add-On")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;




	// �뽬
private:
	UPROPERTY(EditAnywhere)
		UAnimMontage* DashMontage;



private: // ����
	UPROPERTY(VisibleAnywhere)
		class USceneComponent*    ArrowGroup;
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent*    Arrows[(int32)EParkourArrowType::Max];

public:
	// AActor::TakeDamage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator, AActor * DamageCauser) override;


private:
	UFUNCTION()
		void  OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

public:
	struct FDamageData
	{
		float  power = 0.0f;
		class  ACharacter* Character;
		class  AActor*     Causer;
		struct FActionDamageEvent* Event;
	};
	FDamageData Damage;




private:
	void  OnAvoid();
	void  BackStep();
	void  Roll();
	void  Hitted();\
	void  Dead();
private:  // ��Ƽ���̿��� ȣ��
	virtual void End_Avoid() override;
	virtual void End_Hit()   override;
	virtual void End_Dead()  override;

private:
	void OnRightButton();
	void OffRightButton();

private:
	void OnZButton();
	void OffZButton();

private:
	void OnXButton();
	void OffXButton();
private:
	void SetZooming();


private:
	void  CreateParkourArrow();



private:
	void RepeatZKeyAction();
	FTimerHandle TimerHandle;
	bool bZKeyHeld = false; // Z Ű�� ���ȴ��� ���θ� ��Ÿ��

	private:
		void RepeatXKeyAction();
		bool bXKeyHeld = false; // Z Ű�� ���ȴ��� ���θ� ��Ÿ��
	
public:
	// CUserWidget_Stamina Ŭ������ ������ ����
	UCUserWidget_Stamina* HPWidget;
public:
	void UpdateHealthAndStaminaOnWidget(float CurrentHealth, float MaxHealth, float CurrentStamina, float MaxStamina);


private:
	UPROPERTY(EditDefaultsOnly, Category = "assassination ")
		class  UWidgetComponent*  assassinationWidget;

	private:
		UPROPERTY(EditDefaultsOnly, Category = "Audio")
			USoundBase* LandingSound;
	private:
		UPROPERTY(EditDefaultsOnly, Category = "Audio")
			USoundBase* BlockingSound;
	
	FTimerHandle BlockTimerHandle;

	
protected:
	UPROPERTY(EditAnywhere, Category = "Block")
	UAnimMontage* BlockingMontage;
	UPROPERTY(EditAnywhere, Category = "Block")
	UAnimMontage* SuccessBlockingMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* BossGroggyMontage;


	









	
protected:	// inventory
	UPROPERTY()
		ACSTutorialHUD* HUD;

		UPROPERTY(VisibleAnywhere, Category = "Character | IInteraction")
			TScriptInterface<IInteractionInterface> TargetInteractable;
	
	UPROPERTY(VisibleAnywhere, Category = "Character | Iventory")
		UInventoryComponent* PlayerInventory;

		float InteractionCheckFrequency;

		float InteractionCheckDistance;

		FTimerHandle TimerHandle_Interaction;

		FInteractionData InteractionData;

	    void ToggleMenu();


	
private:
	void OnGamePause();
	bool bHideGamePause;
private:
	UPROPERTY()
	UUserWidget* MenuWidget;





	void PerformInteractionCheck();
		void FoundInteractable(AActor* NewInteractable);
		void NoInteractableFound();
		void BeginInteract();
		void EndInteract();
		void Interact();


		void StartBlock();
		void EndBlock();
		bool bIsBlocking = false;
	

public:
	void SavePlayerData();
	void LoadPlayerData();

private:
	TMap<FName, float> SkillCooldowns; 
	TMap<FName, FTimerHandle> CooldownTimers; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CooldownWidgetClass; // 쿨타임 위젯 클래스


	FTimerHandle CooldownUpdateTimerHandle;
   
private:
	void ResetCooldown(FName SkillName);
	void SetSkillCooldown(FName SkillName, float CooldownTime);
	void UpdateCooldowns();
	bool IsSkillOnCooldown(FName SkillName) const;


public:
	void PerformStealthKill();
	virtual void StealthKill() override;

	UPROPERTY(EditAnywhere)
	UAnimMontage* StealthMontage;

private:
	// Function to adjust the camera boom length
	void AdjustCameraBoomLength(float DeltaTime);

	// Desired arm length to use when there are no collisions
	float DesiredArmLength;

	// Function to check for overlaps between the camera and the character
	bool CheckForCameraObstruction(FHitResult& HitResult);


	float MaxCameraArmLength = 500.0f;
	float ProbeSize = 12.0;

};
