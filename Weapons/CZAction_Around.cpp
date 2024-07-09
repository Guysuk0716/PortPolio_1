// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CZAction_Around.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/AddOns/CBlackHole.h"


UCZAction_Around::UCZAction_Around()
{

}

void UCZAction_Around::Pressed()
{
	Super::Pressed();  // bInAction = true;

	CheckTrue(State->IsZActionMode());
	State->OnZActionMode();
	PrintLine();
	ActionData.DoAction(Owner);
}

void UCZAction_Around::Begin_ZAction_Implementation()
{
	Super::Begin_ZAction_Implementation();

	FActorSpawnParameters params;

	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	FTransform transform;

	FVector ownerLocation = Owner->GetActorLocation();
	ownerLocation += Owner->GetActorRotation().RotateVector(ObjectLocation);

	transform.SetLocation(ownerLocation);
	transform.SetRotation(FQuat(Owner->GetActorRotation()));


	Owner->GetWorld()->SpawnActor<ACBlackHole>(ObjectClass, transform, params);
}

void UCZAction_Around::End_ZAction_Implementation()
{
	Super::End_ZAction_Implementation();

	State->OffZActionMode();
	State->SetIdleMode();

	Movement->Move();
}
