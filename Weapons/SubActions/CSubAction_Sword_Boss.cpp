// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/SubActions/CSubAction_Sword_Boss.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/CSword.h"

UCSubAction_Sword_Boss::UCSubAction_Sword_Boss()
{
	
}

void UCSubAction_Sword_Boss::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSubActionMode());

	Super::Pressed();

	State->SetActionMode();
	State->OnSubActionMode();

		CLog::Print("SubAction Boss Pressed");

	

	ActionData.DoAction(Owner);
}
// Auar�� Spawn
void UCSubAction_Sword_Boss::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();
	

	CLog::Print("SubAction Boss Begin SubAction");
	

	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector baseLocation = Owner->GetActorLocation() + FVector(-700.0f,0.0f,0.0f);
	FRotator baseRotation = Owner->GetActorRotation();
	FVector offset = FVector(200.0f, 0.0f, 0.0f); // 각 액터 간의 오프셋을 설정

	int32 numActorsToSpawn = 7; // 스폰할 액터의 수를 지정

	for (int32 i = 0; i < numActorsToSpawn; ++i)
	{
		FTransform transform;

		FVector spawnLocation = baseLocation + baseRotation.RotateVector(SwordLocation) + offset * i;
		transform.SetLocation(spawnLocation);
		transform.SetRotation(FQuat(baseRotation));

		Owner->GetWorld()->SpawnActor<ACSword>(SwordClass, transform, params);
	}
	


	


	

	

}

void UCSubAction_Sword_Boss::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	// State
	State->SetIdleMode();
	State->OffSubActionMode();

	// Movement
	Movement->Move();
	Movement->DisableFixedCamera();


}