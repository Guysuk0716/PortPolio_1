#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	this->NodeName = "Melee";
	this->Interval = 0.1f;
	this->RandomDeviation = 0.0f;
}
void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* contoller = Cast<ACAIController>(OwnerComp.GetOwner());  // BP Cast AI Controller
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(contoller->GetPawn());             // BP Cast Enemy

	// BP���� �����Լ� Tick AIȣ��

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);


	if (state->IsHitMode())
	{
		behavior->SetHittedMode();
		return;
	}
	ACharacter* target = behavior->GetTarget();
	if (target == nullptr)
	{
		behavior->SetPatrolMode();
		return;
	}

	float distance = ai->GetDistanceTo(target);
	if (distance < ActionRange)
	{
		behavior->SetActionMode();
		return;

	}
	behavior->SetApproachMode();

	


}