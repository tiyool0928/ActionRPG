// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player1Anim.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPROJECT_API UPlayer1Anim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	//플레이어 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayAnim)
		float speed = 0;
	//플레이어 좌우 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayAnim)
		float direction = 0;
	//재생할 구르기 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* DodgeRollMontage;
	//재생할 일반공격 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* NormalAttackMontage;
	//재생할 대쉬공격 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* DashAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* Skill1Montage;
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* Skill2Montage;
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* Skill3Montage;
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* Skill4Montage;
	//구르기 애니메이션 재생
	void PlayDodgeRollAnim();
	//일반공격 애니메이션 재생
	void PlayNormalAttackAnim();
	//대쉬공격 애니메이션 재생
	void PlayDashAttackMontage();
	//스킬1 애니메이션 재생
	void PlaySkill1Montage();
	void PlaySkill2Montage();
	void PlaySkill3Montage();
	void PlaySkill4Montage();

	//매 프레임 갱신
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
