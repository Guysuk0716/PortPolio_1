#include "Components/CStateComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCStateComponent::UCStateComponent()
{

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
    // InGame���� Parent�� ���� �ü� ���� : ������(X)
    OwnerCharacter = Cast<ACharacter>(GetOwner());

}

void UCStateComponent::SetIdleMode()
{
    ChangeType(EStateType::Idle);
}

void UCStateComponent::SetBackStepMode()
{
    ChangeType(EStateType::BackStep);
}
void UCStateComponent::SetRollMode()
{
    ChangeType(EStateType::Roll);
}

void UCStateComponent::SetEquipMode()
{
    ChangeType(EStateType::Equip);
}

void UCStateComponent::SetHitMode()
{
    ChangeType(EStateType::Hit);
}

void UCStateComponent::SetActionMode()
{
    ChangeType(EStateType::Action);
}

void UCStateComponent::SetDeadMode()
{
    ChangeType(EStateType::Dead);
}

void UCStateComponent::SetBlockMode()
{
    ChangeType(EStateType::Block);
}

// Noti������ ó�������ϰ� 
void UCStateComponent::OnSubActionMode()
{
    bInSubActionMode = true;
}
void UCStateComponent::OffSubActionMode()
{
    bInSubActionMode = false;
}
void UCStateComponent::OnZActionMode()
{
	bInZActionMode = true;
}
void UCStateComponent::OffZActionMode()
{
	bInZActionMode = false;
}
void UCStateComponent::OnXActionMode()
{
	bInXActionMode = true;
}
void UCStateComponent::OffXActionMode()
{
	bInXActionMode = false;
}
void UCStateComponent::ChangeType(EStateType InType)
{
    EStateType prevType = Type;
    Type = InType;
    if (OnStateTypeChanged.IsBound())
        OnStateTypeChanged.Broadcast(prevType, Type);
}

