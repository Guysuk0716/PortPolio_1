// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CZAction_Boss_Sword.h"
#include "Global.h"
#include "AddOns/CZone.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"


UCZAction_Boss_Sword::UCZAction_Boss_Sword()
{
}

void UCZAction_Boss_Sword::Pressed()
{
	Super::Pressed();

	CheckTrue(State->IsZActionMode());
	State->OnZActionMode();
	PrintLine();
	ActionData.DoAction(Owner);
}

void UCZAction_Boss_Sword::Begin_ZAction_Implementation()
{
	Super::Begin_ZAction_Implementation();

	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform transform;

	FVector baseLocation = Owner->GetActorLocation();
	FRotator baseRotation = Owner->GetActorRotation();
	
		transform.SetLocation(baseLocation);
		transform.SetRotation(FQuat(baseRotation));

		Owner->GetWorld()->SpawnActor<ACZone>(ZoneClass, transform, params);
	
}

void UCZAction_Boss_Sword::End_ZAction_Implementation()
{
	Super::End_ZAction_Implementation();

	State->OffZActionMode();
	State->SetIdleMode();

	Movement->Move();
}
