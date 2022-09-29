// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */

UCLASS(config = game)
class ACTIONPROJECT_API ABossAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
		class UBlackboardData* BBAsset;
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY(transient)
		class UBlackboardComponent* BlackboardComp;

public:
	ABossAIController();
	virtual void OnPossess(APawn* InPawn) override;				//폰에 AIController를 바인딩
	virtual void OnUnPossess() override;						//AIController 언바이딩

	static const FName HomePosKey;								//본인 위치
	static const FName PatrolPosKey;							//이동할 위치
	static const FName Player1Key;								//Player 오브젝트 블랙보드 키 값
	static const FName IsInAttackRangeKey;						//공격할 수 있는 지
	static const FName IsNarrowRotGapKey;						//target과 마주치고 있는 지
	static const FName RandomAttackNumKey;						//어택 패턴 랜덤 변수
};
