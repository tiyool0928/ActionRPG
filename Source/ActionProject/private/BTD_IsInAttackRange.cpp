// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsInAttackRange.h"
#include "BossAIController.h"
#include "EnemyBoss.h"
#include "ActionPlayer1.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTD_IsInAttackRange::UBTD_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTD_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == controllingPawn)
		return false;

	auto target = Cast<AActionPlayer1>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABossAIController::Player1Key));
	if (target == nullptr)
		return false;

	bResult = (target->GetDistanceTo(controllingPawn) <= 400.0f);

	UE_LOG(LogTemp, Warning, TEXT("%.2f"), target->GetDistanceTo(controllingPawn));
	return bResult;
}
