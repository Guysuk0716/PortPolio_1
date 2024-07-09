#include "Components/CMovementComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCMovementComponent::UCMovementComponent()
	: CurrentStamina(100.0f), // �ʱ� ���¹̳� ����
	MaxStamina(100.0f),    // �ִ� ���¹̳� ����
	StaminaRecoveryRate(5.0f), // ���¹̳� ȸ�� �ӵ� ����
	DashStaminaCost(20.0f) // ��� �� ���¹̳� �Ҹ�
{

}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	// InGame���� Parent�� ���� �ü� ���� : ������(X)
	OwnerCharacter = Cast<ACharacter>(GetOwner());


}

void UCMovementComponent::OnMoveForward(float InAxis)
{
	CheckFalse(bCanMove);      // PichYawRoll
	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector  direction = FQuat(rotator).GetForwardVector();

	if (bTopViewCamera)
		direction = FVector::XAxisVector;

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnMoveRight(float InAxis)
{
	CheckFalse(bCanMove);      // PichYawRoll
	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector  direction = FQuat(rotator).GetRightVector();

	if (bTopViewCamera)
		direction = FVector::YAxisVector;
	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnHorizontalLook(float InAxis)
{
	CheckTrue(bFixedCamera);
	OwnerCharacter->AddControllerYawInput(InAxis*HorizontalLook*GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::OnVerticalLook(float InAxis)
{
	CheckTrue(bFixedCamera);
	OwnerCharacter->AddControllerPitchInput(InAxis*VerticalLook*GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

}

void UCMovementComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];

}

void UCMovementComponent::OnDashing()
{
	if (CurrentStamina >= DashStaminaCost)
	{
		const FVector ForwardDir = OwnerCharacter->GetActorForwardVector().GetSafeNormal(); // ĳ������ ���� ���͸� �����ɴϴ�.
		const FVector DashDestination = OwnerCharacter->GetActorLocation() + ForwardDir * DashDistance; // ����� ��ġ ���

		FHitResult HitResult;
		OwnerCharacter->SetActorLocation(DashDestination, true, &HitResult); // ĳ������ ��ġ�� ����� ��ġ�� �̵���ŵ�ϴ�.

		if (DashMontage)
		{
			OwnerCharacter->PlayAnimMontage(DashMontage, 1, NAME_None);
		}

		if (GhostTrailClass)
		{
			GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, OwnerCharacter);
		}

		// ���¹̳��� ���ҽ�ŵ�ϴ�.
		CurrentStamina -= DashStaminaCost;
	}
	else
	{
		// ���¹̳��� ������ ��� ��ø� ������� �ʽ��ϴ�.
		UE_LOG(LogTemp, Warning, TEXT("Not enough stamina to dash!"));
	}

}
void UCMovementComponent::OffDashing()
{

	if (!!GhostTrail)
		GhostTrail->Destroy();
}

void UCMovementComponent::RecoverStamina(float DeltaTime)
{
	CurrentStamina += StaminaRecoveryRate * DeltaTime;
	// ���¹̳��� �ִ�ġ�� �ʰ����� �ʵ��� �մϴ�.
	CurrentStamina = FMath::Min(CurrentStamina, MaxStamina);
}

void UCMovementComponent::SetStamina(float InStamina)
{
	CurrentStamina = FMath::Clamp(InStamina, 0.0f, MaxStamina);
}

