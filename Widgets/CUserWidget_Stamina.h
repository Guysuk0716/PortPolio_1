// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Stamina.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCUserWidget_Stamina : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Health ������ ������Ʈ�ϴ� �Լ�
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealth(float CurrentHealth, float MaxHealth);

	// Stamina ������ ������Ʈ�ϴ� �Լ�
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateStamina(float CurrentStamina, float MaxStamina);

	UFUNCTION(BlueprintImplementableEvent)
	void HighlightFistImage(bool bHighlight);

	UFUNCTION(BlueprintImplementableEvent)
	void HighlightSwordImage(bool bHighlight);

	UFUNCTION(BlueprintImplementableEvent)
	void HighlightHammerImage(bool bHighlight);

	UFUNCTION(BlueprintImplementableEvent)
	void HighlightMagicImage(bool bHighlight);

	UFUNCTION(BlueprintImplementableEvent)
	void HighlightBowImage(bool bHighlight);

public:
	UFUNCTION(BlueprintCallable, Category = "Cooldown")
	void UpdateCooldown(FName SkillName, float CooldownTime);

	UFUNCTION(BlueprintImplementableEvent, Category = "Cooldown")
	void OnCooldownUpdate(FName SkillName, float RemainingTime);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Cooldown")
	void ShowCooldown(FName SkillName);

	UFUNCTION(BlueprintImplementableEvent, Category = "Cooldown")
	void HideCooldown(FName SkillName);


protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


};
