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

	// Sight생성 및 데이터 초기화
	{
		Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
		Sight->SightRadius = 600;
		Sight->LoseSightRadius = 800;
		Sight->PeripheralVisionAngleDegrees = 45;  // 좌우 --> 90, 
		Sight->SetMaxAge(2);
	}
	{
		// 1 : Player, 2: Enemy_AI, 255중립 --> teamID
		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectNeutrals = false;
		Sight->DetectionByAffiliation.bDetectFriendlies = false;
	}

	// Sight인 경우 배열로 구성이 되어있음, 주소를 가지고온다
	Perception->ConfigureSense(*Sight);
	// Sight의 우선순위를 정한다
	Perception->SetDominantSense(*Sight->GetSenseImplementation());

}
void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	// 바인딩
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}
void   ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);        // BP
	SetGenericTeamId(FGenericTeamId(Enemy->GetTeamID()));

	// Enemy에 있는 BB :  Melee,Range등등
	CheckNull(Enemy->GetBehaviorTree());
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	RunBehaviorTree(Enemy->GetBehaviorTree());  // BP

		// 우리가 만든 컴포넌트에 Blackboard를 세팅
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


	//  감지된 Player Setting
	if (actors.Num() > 0)
	{
		CLog::Log(actors[0]->GetName());
		Blackboard->SetValueAsObject("Target", actors[0]);
		return;
	}
	Blackboard->SetValueAsObject("Target", nullptr);
}