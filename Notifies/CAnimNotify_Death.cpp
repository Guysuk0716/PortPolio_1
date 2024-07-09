// Fill out your copyright notice in the Description page of Project Settings.

#include "Notifies/CAnimNotify_Death.h"
#include "Weapons/CDoAction.h"


FString UCAnimNotify_Death::GetNotifyName_Implementation() const
{
	return "Death";
}
void UCAnimNotify_Death::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UE_LOG(LogTemp, Warning, TEXT("Notify function called"));

	if (WidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetClass is not set in UCAnimNotify_Clear"));
		return;
	}

	UWorld* World = MeshComp->GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is not found in UCAnimNotify_Clear"));
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is not found in UCAnimNotify_Clear"));
		return;
	}

	// 기존 위젯 제거
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	// 새 위젯 생성 및 뷰포트에 추가
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
	if (NewWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NewWidget could not be created in UCAnimNotify_Clear"));
		return;
	}

	NewWidget->AddToViewport();

	PlayerController->bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(NewWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
}