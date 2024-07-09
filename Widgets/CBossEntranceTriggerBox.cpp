// Fill out your copyright notice in the Description page of Project Settings.


#include "CBossEntranceTriggerBox.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CPlayer.h"
#include "Components/InventoryComponent.h"

ACBossEntranceTriggerBox::ACBossEntranceTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ACBossEntranceTriggerBox::OnOverlapBegin);
}

void ACBossEntranceTriggerBox::BeginPlay()
{
	Super::BeginPlay();
}

void ACBossEntranceTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (OtherActor == PlayerCharacter && DoesPlayerHaveRequiredItem(OtherActor))
		{
			ACPlayer* Player = Cast<ACPlayer>(PlayerCharacter);
			if (Player)
			{
				Player->SavePlayerData();
			}
			UGameplayStatics::OpenLevel(this, LevelNameToLoad);
		}
	}
}

bool ACBossEntranceTriggerBox::DoesPlayerHaveRequiredItem(AActor* PlayerActor)
{
	ACPlayer* Player = Cast<ACPlayer>(PlayerActor);
	if (Player && Player->GetInventory())
	{
		return Player->GetInventory()->HasItemWithID("test_001");
	}
	return false;
}