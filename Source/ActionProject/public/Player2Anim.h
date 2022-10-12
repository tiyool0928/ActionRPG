// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player2Anim.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPROJECT_API UPlayer2Anim : public UAnimInstance
{
	GENERATED_BODY()

public:
	//플레이어 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float speed = 0;
	//플레이어 좌우 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float direction = 0;

	//재생할 구르기 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* DodgeRollMontage;
	//재생할 기본공격 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* NormalAttackMontage;

	//구르기 애니메이션 재생
	void PlayDodgeRollAnim();
	//기본공격 애니메이션 재생
	void PlayNormalAttackAnim();

	//매 프레임 갱신
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
