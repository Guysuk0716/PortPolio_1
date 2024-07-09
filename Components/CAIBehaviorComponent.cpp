#include "Components/CAIBehaviorComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

UCAIBehaviorComponent::UCAIBehaviorComponent()
{

}

void UCAIBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////////////////////
bool UCAIBehaviorComponent::IsWaitMode()
{
	return GetType() == EAIStateType::Wait;
}

bool UCAIBehaviorComponent::IsApproachMode()
{
	return GetType() == EAIStateType::Approach;
}

bool UCAIBehaviorComponent::IsActionMode()
{
	return GetType() == EAIStateType::Action;
}

bool UCAIBehaviorComponent::IsPatrolMode()
{
	return GetType() == EAIStateType::Patrol;
}

bool UCAIBehaviorComponent::IsHittedMode()
{
	return GetType() == EAIStateType::Hitted;
}

bool UCAIBehaviorComponent::IsAvoidMode()
{
	return GetType() == EAIStateType::Avoid;
}

bool UCAIBehaviorComponent::IsWaveMode()
{
	return GetType() == EAIStateType::Wave;
}

bool UCAIBehaviorComponent::IsDeadMode()
{
	return GetType() == EAIStateType::Dead;
}
///////////////////////////////////////////////////////////////////////
void UCAIBehaviorComponent::SetWaitMode()
{
	ChangeType(EAIStateType::Wait);

}

void UCAIBehaviorComponent::SetApproachMode()
{
	ChangeType(EAIStateType::Approach);

}

void UCAIBehaviorComponent::SetActionMode()
{
	ChangeType(EAIStateType::Action);

}

void UCAIBehaviorComponent::SetPatrolMode()
{
	ChangeType(EAIStateType::Patrol);

}

void UCAIBehaviorComponent::SetHittedMode()
{
	ChangeType(EAIStateType::Hitted);

}

void UCAIBehaviorComponent::SetAvoidMode()
{
	ChangeType(EAIStateType::Avoid);
}

void UCAIBehaviorComponent::SetWaveMode()
{
	ChangeType(EAIStateType::Wave);

}

void UCAIBehaviorComponent::SetDeadMode()
{
	ChangeType(EAIStateType::Dead);
}
//////////////////////////////////////////////////////////////////////
ACharacter * UCAIBehaviorComponent::GetTarget()
{
	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
}
FVector UCAIBehaviorComponent::GetPatrolLocation()
{
	return Blackboard->GetValueAsVector(PatrolLocationKey);
}
void UCAIBehaviorComponent::SetPatrolLocation(const FVector & InLocation)
{
	Blackboard->SetValueAsVector(PatrolLocationKey, InLocation);
}
FVector UCAIBehaviorComponent::GetAvoidLocation()
{
	return Blackboard->GetValueAsVector(AvoidLocationKey);
}
EAIStateType UCAIBehaviorComponent::GetType()
{
	return (EAIStateType)Blackboard->GetValueAsEnum(AIStateTypeKey);
}

void UCAIBehaviorComponent::ChangeType(EAIStateType InType)
{
	EAIStateType prevType = GetType();
	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);//
	if (OnAIStateTypeChanged.IsBound())
		OnAIStateTypeChanged.Broadcast(prevType, InType);
}

bool UCAIBehaviorComponent::HasTarget()
{
	return (GetTarget() != nullptr);
}

void UCAIBehaviorComponent::SetWaveModeWithDelay()
{
	// 3초 후 SetActionMode 호출
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCAIBehaviorComponent::SetActionMode, 3.0f, false);
}

void UCAIBehaviorComponent::SetActionModeWithDelay()
{
	// 3초 후 SetWaveMode 호출
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCAIBehaviorComponent::SetWaveMode, 3.0f, false);
}

