// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack2.h"
#include "BossAIController.h"
#include "EnemyBoss.h"

UBTT_Attack2::UBTT_Attack2()
{
	bNotifyTick = true;
	isAttacking = false;
}

EBTNodeResult::Type UBTT_Attack2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto enemyBoss = Cast<AEnemyBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (enemyBoss == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	enemyBoss->Attack2();
	isAttacking = true;
	GetWorld()->GetTimerManager().SetTimer(AttackDelayHandle, this, &UBTT_Attack2::EndAttack, 1.5f, true);

	return EBTNodeResult::InProgress;
}

void UBTT_Attack2::EndAttack()
{
	isAttacking = false;
}

void UBTT_Attack2::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecond)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSecond);
	if (!isAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
