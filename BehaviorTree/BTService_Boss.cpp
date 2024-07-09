#include "BehaviorTree/BTService_Boss.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/BossController.h"
#include "Characters/CEnemy_Boss.h"
#include "Components/CStateComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Weapons/CDoAction.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"


UBTService_Boss::UBTService_Boss()
{
	this->NodeName = "Boss";
	this->Interval = 0.1f;
	this->RandomDeviation = 0.0f;
	


}
void UBTService_Boss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ABossController* contoller = Cast<ABossController>(OwnerComp.GetOwner());  // BP Cast AI Controller
	ACEnemy_Boss*     ai = Cast<ACEnemy_Boss>(contoller->GetPawn());             // BP Cast Enemy
	

	// BP���� �����Լ� Tick AIȣ��

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	
	ACharacter* target = behavior->GetTarget();

	
	if(state->IsDeadMode())
	{
		behavior->SetDeadMode();
	}
	if (state->IsHitMode())
	{
		behavior->SetHittedMode();
		return;
	}
	if (state->IsActionMode())
	{
		// 공격 중이면 다른 행동을 하지 않도록 함
		return;
	}
	
	if (target == nullptr)
	{
		behavior->SetApproachMode();
		return;
	}
	float distance = ai->GetDistanceTo(target);
	if( distance >= 600 && distance <= 1000)
	{
		behavior->SetPatrolMode();
		return;
		
	}
	if (distance < ActionRange)
	{
		behavior->SetActionMode();
		return;
		
	}
	behavior->SetApproachMode();
}


