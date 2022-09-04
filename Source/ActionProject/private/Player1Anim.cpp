// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1Anim.h"
#include "ActionPlayer1.h"

void UPlayer1Anim::PlayDodgeRollAnim()
{
	if (DodgeRollMontage != nullptr)
	{
		Montage_Play(DodgeRollMontage);
	}
}

void UPlayer1Anim::PlayNormalAttackAnim()
{
	if (NormalAttackMontage != nullptr)
	{
		Montage_Play(NormalAttackMontage);
	}
}

void UPlayer1Anim::PlayDashAttackMontage()
{
	if (DashAttackMontage != nullptr)
	{
		Montage_Play(DashAttackMontage);
	}
}

void UPlayer1Anim::PlaySkill1Montage()
{
	if (Skill1Montage != nullptr)
	{
		Montage_Play(Skill1Montage);
	}
}

void UPlayer1Anim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//플레이어 이동속도 참조
	auto ownerPawn = TryGetPawnOwner();
	auto player = Cast<AActionPlayer1>(ownerPawn);
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
