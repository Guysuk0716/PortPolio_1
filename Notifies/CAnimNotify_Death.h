// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Widgets/CUserWidget_Stamina.h"
#include "CAnimNotify_Death.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCAnimNotify_Death : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY()
	UCUserWidget_Stamina* CurrentWidget;
};
