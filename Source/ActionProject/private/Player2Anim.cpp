// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2Anim.h"
#include "ActionPlayer2.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayer2Anim::PlayDodgeRollAnim()
{
	if (DodgeRollMontage != nullptr)
	{
		Montage_Play(DodgeRollMontage);
	}
}

void UPlayer2Anim::PlayNormalAttackAnim()
{
	if (NormalAttackMontage != nullptr)
	{
		Montage_Play(NormalAttackMontage);
	}
}

void UPlayer2Anim::PlayDashAttackAnim()
{
	if (DashAttackMontage != nullptr)
	{
		Montage_Play(DashAttackMontage);
	}
}

void UPlayer2Anim::PlaySkill1AttackAnim()
{
	if (Skill1AttackMontage != nullptr)
	{
		Montage_Play(Skill1AttackMontage);
	}
}

void UPlayer2Anim::PlaySkill2AttackAnim()
{
	if (Skill2AttackMontage != nullptr)
	{
		Montage_Play(Skill2AttackMontage);
	}
}

void UPlayer2Anim::PlaySkill3AttackAnim()
{
	if (Skill3AttackMontage != nullptr)
	{
		Montage_Play(Skill3AttackMontage);
	}
}

void UPlayer2Anim::PlaySkill4AttackAnim()
{
	if (Skill4AttackMontage != nullptr)
	{
		Montage_Play(Skill4AttackMontage);
	}
}

void UPlayer2Anim::PlayUltimateAttackAnim()
{
	if (UltimateAttackMontage != nullptr)
	{
		Montage_Play(UltimateAttackMontage);
	}
}

void UPlayer2Anim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//플레이어 이동속도 참조
	auto ownerPawn = TryGetPawnOwner();
	auto player = Cast<AActionPlayer2>(ownerPawn);
	//캐스팅 성공 시
	if (player)
	{
		//플레이어의 이동속도
		FVector velocity = player->GetVelocity();
		//플레잉어의 전방 벡터
		FVector forwardVector = player->GetActorForwardVector();
		//speed에 할당
		speed = FVector::DotProduct(forwardVector, velocity);
		//좌우 속도 할당
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);
	}
}


