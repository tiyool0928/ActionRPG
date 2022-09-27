// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack3.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPROJECT_API UBTT_Attack3 : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_Attack3();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FTimerHandle AttackDelayHandle;

	void EndAttack();

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecond) override;

private:
	bool isAttacking;

	
};
