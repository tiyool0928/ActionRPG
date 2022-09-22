// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TurnToPlayer.h"
#include "BossAIController.h"
#include "EnemyBoss.h"
#include "ActionPlayer1.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_TurnToPlayer::UBTT_TurnToPlayer()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTT_TurnToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto enemyBoss = Cast<AEnemyBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == enemyBoss)
		return EBTNodeResult::Failed;

	auto Target = Cast<AActionPlayer1>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABossAIController::Player1Key));
	if (nullptr == Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL Target"));
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - enemyBoss->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	//enemyBoss->SetActorRotation(TargetRot);
	enemyBoss->SetActorRotation(FMath::RInterpTo(enemyBoss->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));
	//UE_LOG(LogTemp, Warning, TEXT("%.2f, %.2f"), LookVector.X, LookVector.Y);

	return EBTNodeResult::Succeeded;
}
