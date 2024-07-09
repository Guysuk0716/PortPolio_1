#include "BehaviorTree/CBTTaskNode_Patrol.h"
#include "Global.h"
#include "CPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
    NodeName = "Patrol";
    bNotifyTick = true;    // True로 설정
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
    if (!controller)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get AI controller"));
        return EBTNodeResult::Failed;
    }

    ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
    if (!ai)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get AI pawn"));
        return EBTNodeResult::Failed;
    }

    UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
    if (!behavior)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get AI behavior component"));
        return EBTNodeResult::Failed;
    }

    if (!!ai->GetPatrolPath())  // UI Setting Check
    {
        FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
        behavior->SetPatrolLocation(moveToPoint); // BB PatrolLocation

        DrawDebug(ai->GetWorld(), moveToPoint);
        
        return EBTNodeResult::InProgress;
    }

    // 현재 위치를 기준으로 랜덤한 탐색 지점을 선택합니다.
    UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
    if (!navSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get navigation system"));
        return EBTNodeResult::Failed;
    }

    FVector location = ai->GetActorLocation();
    FNavLocation point(location);

    // 무한 루프를 방지하기 위해 시도 횟수 제한을 둡니다.
    const int MaxAttempts = 20; // 시도 횟수 증가
    int attempts = 0;
    float adjustedRadius = RandomRadius;
    while (attempts < MaxAttempts)
    {
        if (navSystem->GetRandomPointInNavigableRadius(location, adjustedRadius, point))
            break;
        
        adjustedRadius *= 1.5f; // 시도 실패 시 반경 증가
        attempts++;
    }

    if (attempts >= MaxAttempts)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find a random point in navigable radius after %d attempts with final radius %f"), MaxAttempts, adjustedRadius);
        return EBTNodeResult::Failed;
    }

    behavior->SetPatrolLocation(point.Location);
    DrawDebug(ai->GetWorld(), point.Location);
    return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
    if (!controller)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get AI controller"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
    if (!ai)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get AI pawn"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
    if (!behavior)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get AI behavior component"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    FVector location = behavior->GetPatrolLocation();

    EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, AcceptanceDistance, true);

    switch (result)
    {
    case EPathFollowingRequestResult::Failed:
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        break;
    case EPathFollowingRequestResult::AlreadyAtGoal:
    {
        if (ai->GetPatrolPath())
            ai->GetPatrolPath()->UpdateIndex();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
    break;
    }
}

void UCBTTaskNode_Patrol::DrawDebug(UWorld* InWorld, FVector InLocation)
{
    if (bDebugMode)
        DrawDebugSphere(InWorld, InLocation, 25, 25, FColor::Green, true, 5);
}