// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossAnim.h"
#include "EnemyBoss.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//플레이어 이동속도 참조
	auto ownerPawn = TryGetPawnOwner();
	auto enemyBoss = Cast<AEnemyBoss>(ownerPawn);
	//캐스팅 성공 시
	if (enemyBoss)
	{
		//플레이어의 이동속도
		FVector velocity = enemyBoss->GetVelocity();
		//플레잉어의 전방 벡터
		FVector forwardVector = enemyBoss->GetActorForwardVector();
		//speed에 할당
		speed = FVector::DotProduct(forwardVector, velocity);
		//좌우 속도 할당
		FVector rightVector = enemyBoss->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);
	}
}
