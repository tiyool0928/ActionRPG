// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ABossAIController::HomePosKey(TEXT("HomePos"));
const FName ABossAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ABossAIController::Player1Key(TEXT("Player1"));
const FName ABossAIController::IsInAttackRangeKey(TEXT("IsInAttackRange"));
const FName ABossAIController::IsNarrowRotGapKey(TEXT("IsNarrowRotGap"));
const FName ABossAIController::RandomAttackNumKey(TEXT("RandAttackNum"));

ABossAIController::ABossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/BluePrints/BB_EnemyBossAI.BB_EnemyBossAI'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/BluePrints/BT_EnemyBossAI.BT_EnemyBossAI'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		BlackboardComp->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}
