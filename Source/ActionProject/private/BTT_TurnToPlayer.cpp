// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TurnToPlayer.h"
#include "BossAIController.h"
#include "EnemyBoss.h"
#include "ActionPlayer1.h"
#include "BasePlayer.h"
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

	auto Target = Cast<ABasePlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABossAIController::Player1Key));
	if (nullptr == Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL Target"));
		return EBTNodeResult::Failed;
	}

	/*FVector LookVector = Target->GetActorLocation() - enemyBoss->GetActorLocation();
	FRotator LookAtRotation = FRotator(0.f, LookVector.Rotation().Yaw, 0.f);
	enemyBoss->SetActorRotation(LookAtRotation);*/

	FVector LookVector = Target->GetActorLocation() - enemyBoss->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	float rotGap = abs(enemyBoss->GetActorRotation().Yaw - TargetRot.Yaw);

	enemyBoss->SetActorRotation(FMath::RInterpTo(enemyBoss->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 5.0f));
	//UE_LOG(LogTemp, Warning, TEXT("rotGap: %.3f"), rotGap);

	if (rotGap <= 20)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ABossAIController::IsNarrowRotGapKey, true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ABossAIController::IsNarrowRotGapKey, false);
	}

	return EBTNodeResult::Succeeded;
}
