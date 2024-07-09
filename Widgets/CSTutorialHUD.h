// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CSTutorialHUD.generated.h"

struct FInteractableData;
class UInteractionWidget;
class UMainMenu;


UCLASS()
class U06_BATTLE_API ACSTutorialHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PauseWidgetClass;



	bool bIsMenuVisible;
	bool bIsPauseMenuVisible;
	//=================================================================
	// FUNCTIONS
	//=================================================================
	ACSTutorialHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void DisplayPauseMenu();
	void HidePauseMenu();
	void TogglePauseMenu();
	
	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData)const;


protected:
	//=================================================================
	// PROPERTIES & VARIABLES
	//=================================================================
	UPROPERTY()
		UMainMenu* MainMenuWidget;
	UPROPERTY()
	UUserWidget* PauseWidget;

	UPROPERTY()
		UInteractionWidget* InteractionWidget;
	//=================================================================
	// FUNCTIONS
	//=================================================================
	virtual void BeginPlay()  override;




};
