// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_RandomAttackPattern.h"
#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_RandomAttackPattern::UBTS_RandomAttackPattern()
{
	NodeName = TEXT("RandomAttackPattern");
	Interval = 1.0f;
	randNum = 0;
}

void UBTS_RandomAttackPattern::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	randNum = FMath::RandRange(0, 1);

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(ABossAIController::RandomAttackNumKey, randNum);
	UE_LOG(LogTemp, Warning, TEXT("%d"), OwnerComp.GetBlackboardComponent()->GetValueAsInt(ABossAIController::RandomAttackNumKey));

	return;
}
