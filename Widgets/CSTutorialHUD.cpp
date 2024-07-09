// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CSTutorialHUD.h"

#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/MainMenu.h"
#include "Widgets/Interaction/InteractionWidget.h"

ACSTutorialHUD::ACSTutorialHUD()
{
}
void ACSTutorialHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (PauseWidgetClass)
	{
		PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);
		PauseWidget->AddToViewport(3);
		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}



void ACSTutorialHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACSTutorialHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ACSTutorialHUD::ToggleMenu()
{
	if(bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void ACSTutorialHUD::DisplayPauseMenu()
{
	if (PauseWidget)
	{
		bIsPauseMenuVisible = true;
		PauseWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACSTutorialHUD::HidePauseMenu()
{
	if (PauseWidget)
	{
		bIsPauseMenuVisible = false;
		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ACSTutorialHUD::TogglePauseMenu()
{
	if(bIsPauseMenuVisible)
	{
		HidePauseMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayPauseMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void ACSTutorialHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACSTutorialHUD::HideInteractionWidget() const
{
		if (InteractionWidget)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	
}

void ACSTutorialHUD::UpdateInteractionWidget(const FInteractableData * InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);

	}
}


