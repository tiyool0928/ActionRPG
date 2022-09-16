// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttackComponent.h"
#include "PlayerMoveComponent.h"
#include "Player1Anim.h"
#include "Components/ArrowComponent.h"
#include "Player1_Skill1.h"
#include "Player1_Skill3.h"
#include "Player1_Skill4.h"
#include "Player1_Skill4Landing.h"
#include "Player1_UltimateBoom.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

UPlayerAttackComponent::UPlayerAttackComponent()
{
	canDamage = false;
	comboCnt = 0;										//처음 공격은 0번째콤보부터
}

void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerAttackComponent::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	//스킬 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Skill1"), IE_Pressed, this, &UPlayerAttackComponent::InputSkill1);
	PlayerInputComponent->BindAction(TEXT("Skill2"), IE_Pressed, this, &UPlayerAttackComponent::InputSkill2);
	PlayerInputComponent->BindAction(TEXT("Skill3"), IE_Pressed, this, &UPlayerAttackComponent::InputSkill3);
	PlayerInputComponent->BindAction(TEXT("Skill4"), IE_Pressed, this, &UPlayerAttackComponent::InputSkill4);
	PlayerInputComponent->BindAction(TEXT("Skill4"), IE_Released, this, &UPlayerAttackComponent::OutputSkill4);
	PlayerInputComponent->BindAction(TEXT("Ultimate"), IE_Pressed, this, &UPlayerAttackComponent::InputUltimate);
}

void UPlayerAttackComponent::NormalAttack()
{
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	//구르고 있으면 스킬, 궁극기 사용중이면 공격X
	if (moveVar->isRollingAnim || me->isSkillAttacking || me->isUltimateAttacking || me->isSkill4Flying) return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->NormalAttackMontage) return;
	const char* comboList[] = { "Attack1_Start", "Attack2_Start","Attack3_Start" };

	if (!anim->Montage_IsPlaying(anim->NormalAttackMontage))	//공격 몽타주가 실행중이지 않을때
	{
		anim->PlayNormalAttackAnim();		//일반공격 애니메이션 on
	}
	else														//공격몽타주가 실행중일때
	{
		anim->PlayNormalAttackAnim();
		anim->Montage_JumpToSection(FName(comboList[comboCnt]), anim->NormalAttackMontage);
	}
	me->isAttacking = true;
}

void UPlayerAttackComponent::DashAttack()
{
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	//구르고 있거나 이미 대쉬공격중 // 스킬공격중이면 공격X
	if (moveVar->isRollingAnim || me->isDashAttacking || me->isSkillAttacking || me->isSkill2Attacking || me->isUltimateAttacking)
		return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->DashAttackMontage) return;

	anim->PlayDashAttackMontage();
	anim->Montage_JumpToSection("Section_Start", anim->DashAttackMontage);
	me->isAttacking = true;
	me->isDashAttacking = true;
	//1초 뒤 대쉬어택 모션 끝
	GetWorld()->GetTimerManager().SetTimer(DashAttackAnimTimerHandle, this, &UPlayerAttackComponent::DashAttackDelay, 1.0f, true);

}

void UPlayerAttackComponent::DashAttackDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(DashAttackAnimTimerHandle);		//대쉬어택 모션타이머 초기화
	me->isAttacking = false;						//공격 끝
	me->isDashAttacking = false;					//대쉬공격 끝
}

void UPlayerAttackComponent::AttackDamageApplyingComp()
{
	canDamage = true;
}

void UPlayerAttackComponent::AttackDamageEndComp()
{
	canDamage = false;

	if (me->isUltimateAttacking)
	{
		FTransform skillPosition = me->skillArrow->GetComponentTransform();
		GetWorld()->SpawnActor<APlayer1_UltimateBoom>(ultimateFactory, skillPosition);
	}
}

void UPlayerAttackComponent::InputSkill1()
{
	//구르기, 대쉬공격, 스킬공격, 궁극기, 쿨타임 중에 사용X
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || me->isDashAttacking || me->isSkillAttacking
		|| me->isSkill2Attacking || me->isUltimateAttacking || me->isCoolTimeSkill1)
		return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill1Montage) return;

	anim->PlaySkill1Montage();
	me->isSkillAttacking = true;
	GetWorld()->GetTimerManager().SetTimer(Skill1CoolTimerHandle, this, &UPlayerAttackComponent::CoolDownSkill1, 1.0f, true);
	me->isCoolTimeSkill1 = true;		//스킬1 쿨타임 on
}

void UPlayerAttackComponent::CreateSkill1EffectComp()
{
	FTransform skillPosition = me->skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer1_Skill1>(skill1Factory, skillPosition);
}

void UPlayerAttackComponent::CoolDownSkill1()
{
	--(me->skill1CoolTime);

	if (me->skill1CoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Skill1CoolTimerHandle);
		me->isCoolTimeSkill1 = false;					//스킬1 사용가능
		me->skill1CoolTime = 4;							//쿨타임 초기화
	}
}

void UPlayerAttackComponent::InputSkill2()
{
	//구르기, 대쉬공격, 스킬공격, 궁극기, 쿨타임 중에 사용X
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || me->isDashAttacking || me->isSkillAttacking
		|| me->isSkill2Attacking || me->isUltimateAttacking || me->isCoolTimeSkill2)
		return;

	if (me->isAttacking)							//공격중에 스킬2를 썼으면
		me->isAttacking = false;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill2Montage) return;

	anim->PlaySkill2Montage();
	me->isSkill2Attacking = true;
	GetWorld()->GetTimerManager().SetTimer(Skill2CoolTimerHandle, this, &UPlayerAttackComponent::CoolDownSkill2, 1.0f, true);
	me->isCoolTimeSkill2 = true;		//스킬2 쿨타임 on

	auto movement = moveComp;
	movement->MaxWalkSpeed = moveVar->runSpeed;
	//충격체 컴포넌트 활성화
	me->skill2BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->skill2EffectComp->SetVisibility(true);
}

void UPlayerAttackComponent::CoolDownSkill2()
{
	--me->skill2CoolTime;

	if (me->skill2CoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Skill2CoolTimerHandle);
		me->isCoolTimeSkill2 = false;						//스킬2 사용가능
		me->skill2CoolTime = 10;							//쿨타임 초기화
	}
}

void UPlayerAttackComponent::InputSkill3()
{
	//구르기, 대쉬공격, 스킬공격, 궁극기, 쿨타임 중에 사용X
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || me->isDashAttacking || me->isSkillAttacking
		|| me->isSkill2Attacking || me->isUltimateAttacking || me->isCoolTimeSkill3)
		return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill3Montage) return;

	anim->PlaySkill3Montage();
	me->isSkillAttacking = true;
	GetWorld()->GetTimerManager().SetTimer(Skill3CoolTimerHandle, this, &UPlayerAttackComponent::CoolDownSkill3, 1.0f, true);
	me->isCoolTimeSkill3 = true;		//스킬3 쿨타임 on
}

void UPlayerAttackComponent::CreateSkill3EffectComp()
{
	UE_LOG(LogTemp, Warning, TEXT("Before Effect"));
	FTransform skillPosition = me->skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer1_Skill3>(skill3Factory, skillPosition);
}

void UPlayerAttackComponent::CoolDownSkill3()
{
	--me->skill3CoolTime;

	if (me->skill3CoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Skill3CoolTimerHandle);
		me->isCoolTimeSkill3 = false;					//스킬3 사용가능
		me->skill3CoolTime = 8;							//쿨타임 초기화
	}
}

void UPlayerAttackComponent::InputSkill4()
{
	//구르기, 대쉬공격, 스킬공격, 궁극기, 쿨타임 중에 사용X
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || me->isDashAttacking || me->isSkillAttacking
		|| me->isSkill2Attacking || me->isUltimateAttacking || me->isCoolTimeSkill4)
		return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill4Montage) return;

	anim->PlaySkill4Montage();
	anim->Montage_JumpToSection("Section_Start", anim->Skill4Montage);
	me->isSkill4Releasing = true;
	me->isSkillAttacking = true;
	me->isSkill4Flying = true;
	GetWorld()->GetTimerManager().SetTimer(Skill4CoolTimerHandle, this, &UPlayerAttackComponent::CoolDownSkill4, 1.0f, true);
	me->isCoolTimeSkill4 = true;		//스킬4 쿨타임 on

	moveComp->SetMovementMode(EMovementMode::MOVE_Flying);		//애니메이션 z축 활성화

	GetWorld()->GetTimerManager().SetTimer(Skill4FeverOnHandle, this, &UPlayerAttackComponent::Skill4FeverOnDelay, 2.0f, true);
	GetWorld()->GetTimerManager().SetTimer(Skill4EndDelayHandle, this, &UPlayerAttackComponent::Skill4EndDelay, 4.0f, true);
}

void UPlayerAttackComponent::OutputSkill4()
{
	if (me->isSkill4Flying)
	{
		me->isSkill4Releasing = false;
	}

	if (!me->isSkill4Releasing) return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill4Montage) return;

	anim->PlaySkill4Montage();
	anim->Montage_JumpToSection("Charge_End", anim->Skill4Montage);
	me->isSkill4Releasing = false;

	GetWorld()->GetTimerManager().SetTimer
	(Skill4EndMotionDelayHandle, this, &UPlayerAttackComponent::Skill4EndMotionDelay, 1.0f, true);

	if (me->skill4FeverTime)
	{
		FTransform skillPosition = me->skillArrow->GetComponentTransform();
		GetWorld()->SpawnActor<APlayer1_Skill4>(skill4Factory, skillPosition);
	}
}

void UPlayerAttackComponent::Skill4FeverOnDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(Skill4FeverOnHandle);		//스킬4 피버타임 시작

	if (me->isSkillAttacking)
	{
		UE_LOG(LogTemp, Warning, TEXT("FeverOn"));
		me->skill4FeverTime = true;
		GetWorld()->GetTimerManager().SetTimer(Skill4FeverOffHandle, this, &UPlayerAttackComponent::Skill4FeverOffDelay, 2.0f, true);
	}
}

void UPlayerAttackComponent::Skill4FeverOffDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(Skill4FeverOffHandle);		//스킬4 피버타임 종료
	UE_LOG(LogTemp, Warning, TEXT("FeverOff"));
	me->skill4FeverTime = false;
}

void UPlayerAttackComponent::Skill4EndDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(Skill4EndDelayHandle);		//스킬4 애니메이션 종료

	if (!me->isSkillAttacking) return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill4Montage) return;

	anim->PlaySkill4Montage();
	anim->Montage_JumpToSection("Charge_End", anim->Skill4Montage);
	me->isSkillAttacking = false;

	GetWorld()->GetTimerManager().SetTimer
	(Skill4EndMotionDelayHandle, this, &UPlayerAttackComponent::Skill4EndMotionDelay, 1.0f, true);
}

void UPlayerAttackComponent::Skill4EndMotionDelay()
{
	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	GetWorld()->GetTimerManager().ClearTimer(Skill4EndMotionDelayHandle);		//스킬4 공격모션 종료
	anim->StopAllMontages(0.2f);
	me->isSkillAttacking = false;
}

void UPlayerAttackComponent::Skill4CanDodgeComp()
{
	me->isSkill4Flying = false;
	moveComp->SetMovementMode(EMovementMode::MOVE_Walking);		//애니메이션 z축 비활성화

	FTransform skillPosition = me->skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer1_Skill4Landing>(skill4LandingFactory, skillPosition);

	if (!me->isSkill4Releasing)
	{
		auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
		GetWorld()->GetTimerManager().ClearTimer(Skill4EndDelayHandle);		//스킬4 애니메이션 종료
		anim->PlaySkill4Montage();
		anim->Montage_JumpToSection("Charge_End", anim->Skill4Montage);

		GetWorld()->GetTimerManager().SetTimer
		(Skill4EndMotionDelayHandle, this, &UPlayerAttackComponent::Skill4EndMotionDelay, 1.0f, true);
	}
}

void UPlayerAttackComponent::CoolDownSkill4()
{
	--me->skill4CoolTime;

	if (me->skill4CoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Skill4CoolTimerHandle);
		me->isCoolTimeSkill4 = false;						//스킬4 사용가능
		me->skill4CoolTime = 15;							//쿨타임 초기화
	}
}

void UPlayerAttackComponent::InputUltimate()
{
	//구르기, 대쉬공격, 스킬공격, 쿨타임 중에 사용X
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || me->isDashAttacking || me->isSkillAttacking || me->isCoolTimeUltimate)
		return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->UltimateMontage) return;

	anim->PlayUltimateMontage();

	me->isUltimateAttacking = true;
	GetWorld()->GetTimerManager().SetTimer(UltimateCoolTimerHandle, this, &UPlayerAttackComponent::CoolDownUltimate, 1.0f, true);
	me->isCoolTimeUltimate = true;		//궁극기 쿨타임 on

	//충격체 컴포넌트 활성화
	me->ultimateBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->ultimateEffectComp->SetVisibility(true);
}

void UPlayerAttackComponent::CoolDownUltimate()
{
	--me->ultimateCoolTime;

	if (me->ultimateCoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(UltimateCoolTimerHandle);
		me->isCoolTimeUltimate = false;						//궁극기 사용가능
		me->ultimateCoolTime = 60;							//쿨타임 초기화
	}
}