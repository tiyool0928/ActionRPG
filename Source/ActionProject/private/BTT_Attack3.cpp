// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack3.h"
#include "BossAIController.h"
#include "EnemyBoss.h"

UBTT_Attack3::UBTT_Attack3()
{
	bNotifyTick = true;
	isAttacking = false;
}

EBTNodeResult::Type UBTT_Attack3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto enemyBoss = Cast<AEnemyBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (enemyBoss == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	enemyBoss->Attack3();
	isAttacking = true;
	GetWorld()->GetTimerManager().SetTimer(AttackDelayHandle, this, &UBTT_Attack3::EndAttack, 4.0f, true);

	return EBTNodeResult::InProgress;
}

void UBTT_Attack3::EndAttack()
{
	isAttacking = false;
}

void UBTT_Attack3::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecond)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSecond);
	if (!isAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
