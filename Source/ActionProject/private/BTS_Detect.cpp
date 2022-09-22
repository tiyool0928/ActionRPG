// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Detect.h"
#include "BossAIController.h"
#include "EnemyBoss.h"
#include "ActionPlayer1.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTS_Detect::UBTS_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTS_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (controllingPawn == nullptr)
	{
		return;
	}

	UWorld* World = controllingPawn->GetWorld();
	FVector Center = controllingPawn->GetActorLocation();
	float DetectRadius = 2000.0f;

	if (World == nullptr) return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, controllingPawn);

	bool bResult = World->OverlapMultiByChannel(OverlapResults,Center,FQuat::Identity,ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),CollisionQueryParam);
	//UE_LOG(LogTemp, Warning, TEXT("before bResult"));

	if (bResult)
	{
		//UE_LOG(LogTemp, Warning, TEXT("after bResult"));
		for (auto const& OverlapResult : OverlapResults)
		{
			//UE_LOG(LogTemp, Warning, TEXT("after for auto"));
			AActionPlayer1* Player = Cast<AActionPlayer1>(OverlapResult.GetActor());
			if (Player && Player->GetController()->IsPlayerController())
			{
				//UE_LOG(LogTemp, Warning, TEXT("after BossIsPlayerController"));
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABossAIController::Player1Key, Player);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, Player->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, controllingPawn->GetActorLocation(), Player->GetActorLocation(), FColor::Blue, false, 0.27f);
				return;
			}
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABossAIController::Player1Key, nullptr);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}
}
