// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyBossAnim.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPROJECT_API UEnemyBossAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	//플레이어 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayAnim)
	float speed = 0;
	//플레이어 좌우 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayAnim)
		float direction = 0;

	//매 프레임 갱신
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
