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

	static const FName HomePosKey;								//본인 위치
	static const FName PatrolPosKey;							//이동할 위치
	static const FName Player1Key;								//Player 오브젝트 블랙보드 키 값
};