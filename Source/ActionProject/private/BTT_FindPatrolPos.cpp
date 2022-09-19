// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindPatrolPos.h"
#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTT_FindPatrolPos::UBTT_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTT_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (controllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(controllingPawn->GetWorld());
	if (navSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ABossAIController::HomePosKey);
	FNavLocation nextPatrol;

	if (navSystem->GetRandomPointInNavigableRadius(origin, 500.0f, nextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ABossAIController::PatrolPosKey, nextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
