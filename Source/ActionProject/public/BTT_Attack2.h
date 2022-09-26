// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack2.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPROJECT_API UBTT_Attack2 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_Attack2();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FTimerHandle AttackDelayHandle;

	void EndAttack();

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecond) override;

private:
	bool isAttacking;
};
