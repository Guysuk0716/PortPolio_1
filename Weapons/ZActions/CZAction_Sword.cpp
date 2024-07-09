
#include "Weapons/ZActions/CZAction_Sword.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/CGhostTrail.h"
#include "Weapons/AddOns/CZAura.h"

void UCZAction_Sword::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsZActionMode());

	Super::Pressed();

	State->SetActionMode();
	State->OnZActionMode();

		CLog::Print("ZAction");
		PrintLine();

	if (!!GhostTrailClass)
		GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);


	ActionData.DoAction(Owner);
}
// Auar¸¦ Spawn
void UCZAction_Sword::Begin_ZAction_Implementation()
{
	Super::Begin_ZAction_Implementation();

	CheckNull(ZAuraClass);


	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector ForwardVector = Owner->GetActorForwardVector();
	FVector SocketLocation = Owner->GetMesh()->GetSocketLocation("Hand_Sword");
	FRotator SocketRotation = FRotator(Owner->GetActorForwardVector().Rotation().Pitch, Owner->GetActorForwardVector().Rotation().Yaw, Owner->GetMesh()->GetSocketRotation("Hand_Sword").Roll);

	FTransform Transform(SocketRotation, SocketLocation);
	Owner->GetWorld()->SpawnActor<ACZAura>(ZAuraClass, Transform, params);

	//FTransform transform;
	//transform.SetLocation(Owner->GetActorLocation());
	//transform.AddToTranslation(ZAuarLocation);
	//transform.SetRotation(FQuat(Owner->GetActorRotation()));
	//
	//
	//Owner->GetWorld()->SpawnActor<ACZAura>(ZAuraClass, transform, params);  // Destory

	


}

void UCZAction_Sword::End_ZAction_Implementation()
{
	Super::End_ZAction_Implementation();

	// State
	State->SetIdleMode();
	State->OffZActionMode();

	// Movement
	Movement->Move();
	Movement->DisableFixedCamera();

	// GhostTrail
	if (!!GhostTrail)
		GhostTrail->Destroy();
}
