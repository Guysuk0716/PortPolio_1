// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CAIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ACAIController::ACAIController()
{

	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &this->Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	// Sight���� �� ������ �ʱ�ȭ
	{
		Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
		Sight->SightRadius = 600;
		Sight->LoseSightRadius = 800;
		Sight->PeripheralVisionAngleDegrees = 45;  // �¿� --> 90, 
		Sight->SetMaxAge(2);
	}
	{
		// 1 : Player, 2: Enemy_AI, 255�߸� --> teamID
		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectNeutrals = false;
		Sight->DetectionByAffiliation.bDetectFriendlies = false;
	}

	// Sight�� ��� �迭�� ������ �Ǿ�����, �ּҸ� ������´�
	Perception->ConfigureSense(*Sight);
	// Sight�� �켱������ ���Ѵ�
	Perception->SetDominantSense(*Sight->GetSenseImplementation());

}
void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	// ���ε�
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}
void   ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);        // BP
	SetGenericTeamId(FGenericTeamId(Enemy->GetTeamID()));

	// Enemy�� �ִ� BB :  Melee,Range���
	CheckNull(Enemy->GetBehaviorTree());
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	RunBehaviorTree(Enemy->GetBehaviorTree());  // BP

		// �츮�� ���� ������Ʈ�� Blackboard�� ����
	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);
	CheckNull(Behavior);
	Behavior->SetBlackboard(Blackboard);


}
void   ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
void  ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;

	Perception->GetCurrentlyPerceivedActors(nullptr, actors);


	//  ������ Player Setting
	if (actors.Num() > 0)
	{
		CLog::Log(actors[0]->GetName());
		Blackboard->SetValueAsObject("Target", actors[0]);
		return;
	}
	Blackboard->SetValueAsObject("Target", nullptr);
}