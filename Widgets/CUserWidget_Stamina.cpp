// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CUserWidget_Stamina.h"
#include "Characters/CPlayer.h"

void UCUserWidget_Stamina::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	
	// Tick에서 수행할 작업을 여기에 작성합니다.
}

void UCUserWidget_Stamina::UpdateCooldown(FName SkillName, float CooldownTime)
{
	OnCooldownUpdate(SkillName, CooldownTime);
}

