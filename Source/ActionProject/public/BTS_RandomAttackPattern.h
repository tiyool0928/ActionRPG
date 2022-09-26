// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_RandomAttackPattern.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPROJECT_API UBTS_RandomAttackPattern : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_RandomAttackPattern();

	int randNum;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
