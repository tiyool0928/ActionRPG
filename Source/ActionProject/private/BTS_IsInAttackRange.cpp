// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_IsInAttackRange.h"
#include "BossAIController.h"
#include "EnemyBoss.h"
#include "ActionPlayer1.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_IsInAttackRange::UBTS_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
	Interval = 1.0f;
}

void UBTS_IsInAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == controllingPawn)
		return;

	auto target = Cast<AActionPlayer1>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABossAIController::Player1Key));
	if (target == nullptr)
		return;

	bool bResult = (target->GetDistanceTo(controllingPawn) <= 400.0f);

	//UE_LOG(LogTemp, Warning, TEXT("%.2f"), target->GetDistanceTo(controllingPawn));
	if (bResult)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ABossAIController::IsInAttackRangeKey, true);
		return;
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ABossAIController::IsInAttackRangeKey, false);
		return;
	}
		
}
