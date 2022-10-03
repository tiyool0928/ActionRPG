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
#include "UI_ActionPlayer1.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

UPlayerAttackComponent::UPlayerAttackComponent()
{
	canDamage = false;
	comboCnt = 0;										//처음 공격은 0번째콤보부터
	isImpacting = false;
	isAttacking = false;
	isSkillAttacking = false;
	isSkill2Attacking = false;
	isSkill4Flying = false;
	isSkill4Releasing = false;
	isUltimateAttacking = false;
	isAttackButtonWhenAttack = false;

	isCoolTimeSkill1 = false;
	maxSkill1CoolTime = 4;								//스킬1 쿨타임 = 4초
	skill1CoolTime = 0;									//0초로 초기화	

	isCoolTimeSkill2 = false;
	maxSkill2CoolTime = 10;								//스킬2 쿨타임 = 10초
	skill2CoolTime = 0;									//0초로 초기화

	isCoolTimeSkill3 = false;
	maxSkill3CoolTime = 8;								//스킬3 쿨타임 = 8초
	skill3CoolTime = 0;									//0초로 초기화

	isCoolTimeSkill4 = false;
	maxSkill4CoolTime = 15;								//스킬4 쿨타임 = 15초
	skill4CoolTime = 0;									//0초로 초기화

	isCoolTimeUltimate = false;
	maxUltimateCoolTime = 60;							//궁극기 쿨타임 = 60초
	ultimateCoolTime = 0;									//0초로 초기화

	skill2Delay = false;
	skill4FeverTime = false;
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
	//구르고 있으면, 스킬, 궁극기 사용중, 피격중이면 공격X
	if (moveVar->isRollingAnim || isSkillAttacking || isSkill4Flying 
		|| isUltimateAttacking || isImpacting) return;

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
	isAttacking = true;
}

void UPlayerAttackComponent::DashAttack()
{
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	//구르고 있거나 이미 대쉬공격중, 스킬공격중, 피격중이면 공격X
	if (moveVar->isRollingAnim || isDashAttacking || isSkillAttacking || isSkill2Attacking 
		|| isUltimateAttacking || isImpacting)
		return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->DashAttackMontage) return;

	anim->PlayDashAttackMontage();
	anim->Montage_JumpToSection("Section_Start", anim->DashAttackMontage);
	isAttacking = true;
	isDashAttacking = true;
	//1초 뒤 대쉬어택 모션 끝
	GetWorld()->GetTimerManager().SetTimer(DashAttackAnimTimerHandle, this, &UPlayerAttackComponent::DashAttackDelay, 1.0f, true);

}

void UPlayerAttackComponent::DashAttackDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(DashAttackAnimTimerHandle);		//대쉬어택 모션타이머 초기화
	isAttacking = false;						//공격 끝
	isDashAttacking = false;					//대쉬공격 끝
}

void UPlayerAttackComponent::AttackDamageApplyingComp()
{
	canDamage = true;
}

void UPlayerAttackComponent::AttackDamageEndComp()
{
	canDamage = false;

	if (isUltimateAttacking)
	{
		FTransform skillPosition = me->skillArrow->GetComponentTransform();
		GetWorld()->SpawnActor<APlayer1_UltimateBoom>(ultimateFactory, skillPosition);
	}
}

void UPlayerAttackComponent::InputSkill1()
{
	//구르기, 대쉬공격, 스킬공격, 궁극기, 쿨타임, 피격중에 사용X
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || isDashAttacking || isSkillAttacking
		|| isSkill2Attacking || isUltimateAttacking || isCoolTimeSkill1 || isImpacting)
		return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill1Montage) return;

	anim->PlaySkill1Montage();
	isSkillAttacking = true;
	skill1CoolTime = maxSkill1CoolTime;
	GetWorld()->GetTimerManager().SetTimer(Skill1CoolTimerHandle, this, &UPlayerAttackComponent::CoolDownSkill1, 1.0f, true);
	isCoolTimeSkill1 = true;		//스킬1 쿨타임 on
}

void UPlayerAttackComponent::CreateSkill1EffectComp()
{
	FTransform skillPosition = me->skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer1_Skill1>(skill1Factory, skillPosition);
}

void UPlayerAttackComponent::CoolDownSkill1()
{
	--skill1CoolTime;
	UUI_ActionPlayer1* OwnerWidget = me->Widget;
	if (OwnerWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Widget"));
		return;
	}
	OwnerWidget->UpdateSkill1CoolTime();

	if (skill1CoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Skill1CoolTimerHandle);
		isCoolTimeSkill1 = false;					//스킬1 사용가능
	}
}

void UPlayerAttackComponent::InputSkill2()
{
	//구르기, 대쉬공격, 스킬공격, 궁극기, 쿨타임, 피격중에 사용X
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || isDashAttacking || isSkillAttacking
		|| isSkill2Attacking || isUltimateAttacking || isCoolTimeSkill2 || isImpacting)
		return;

	if (isAttacking)							//공격중에 스킬2를 썼으면
		isAttacking = false;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill2Montage) return;

	anim->PlaySkill2Montage();
	isSkill2Attacking = true;
	skill2CoolTime = maxSkill2CoolTime;
	GetWorld()->GetTimerManager().SetTimer(Skill2CoolTimerHandle, this, &UPlayerAttackComponent::CoolDownSkill2, 1.0f, true);
	isCoolTimeSkill2 = true;		//스킬2 쿨타임 on

	auto movement = moveComp;
	movement->MaxWalkSpeed = moveVar->runSpeed;
	//충격체 컴포넌트 활성화
	me->skill2BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->skill2EffectComp->SetVisibility(true);
}

void UPlayerAttackComponent::CoolDownSkill2()
{
	--skill2CoolTime;

	UUI_ActionPlayer1* OwnerWidget = me->Widget;
	if (OwnerWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Widget"));
		return;
	}
	OwnerWidget->UpdateSkill2CoolTime();

	if (skill2CoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Skill2CoolTimerHandle);
		isCoolTimeSkill2 = false;						//스킬2 사용가능
	}
}

void UPlayerAttackComponent::InputSkill3()
{
	//구르기, 대쉬공격, 스킬공격, 궁극기, 쿨타임, 피격중에 사용X
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || isDashAttacking || isSkillAttacking
		|| isSkill2Attacking || isUltimateAttacking || isCoolTimeSkill3 || isImpacting)
		return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill3Montage) return;

	anim->PlaySkill3Montage();
	isSkillAttacking = true;
	skill3CoolTime = maxSkill3CoolTime;
	GetWorld()->GetTimerManager().SetTimer(Skill3CoolTimerHandle, this, &UPlayerAttackComponent::CoolDownSkill3, 1.0f, true);
	isCoolTimeSkill3 = true;		//스킬3 쿨타임 on
}

void UPlayerAttackComponent::CreateSkill3EffectComp()
{
	UE_LOG(LogTemp, Warning, TEXT("Before Effect"));
	FTransform skillPosition = me->skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer1_Skill3>(skill3Factory, skillPosition);
}

void UPlayerAttackComponent::CoolDownSkill3()
{
	--skill3CoolTime;

	UUI_ActionPlayer1* OwnerWidget = me->Widget;
	if (OwnerWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Widget"));
		return;
	}
	OwnerWidget->UpdateSkill3CoolTime();

	if (skill3CoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Skill3CoolTimerHandle);
		isCoolTimeSkill3 = false;					//스킬3 사용가능
	}
}

void UPlayerAttackComponent::InputSkill4()
{
	//구르기, 대쉬공격, 스킬공격, 궁극기, 쿨타임, 피격중에 사용X
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || isDashAttacking || isSkillAttacking
		|| isSkill2Attacking || isUltimateAttacking || isCoolTimeSkill4 || isImpacting)
		return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill4Montage) return;

	anim->PlaySkill4Montage();
	anim->Montage_JumpToSection("Section_Start", anim->Skill4Montage);
	isSkill4Releasing = true;
	isSkillAttacking = true;
	isSkill4Flying = true;
	skill4CoolTime = maxSkill4CoolTime;
	GetWorld()->GetTimerManager().SetTimer(Skill4CoolTimerHandle, this, &UPlayerAttackComponent::CoolDownSkill4, 1.0f, true);
	isCoolTimeSkill4 = true;		//스킬4 쿨타임 on

	moveComp->SetMovementMode(EMovementMode::MOVE_Flying);		//애니메이션 z축 활성화

	GetWorld()->GetTimerManager().SetTimer(Skill4FeverOnHandle, this, &UPlayerAttackComponent::Skill4FeverOnDelay, 2.0f, true);
	GetWorld()->GetTimerManager().SetTimer(Skill4EndDelayHandle, this, &UPlayerAttackComponent::Skill4EndDelay, 4.0f, true);
}

void UPlayerAttackComponent::OutputSkill4()
{
	if (isSkill4Flying)
	{
		isSkill4Releasing = false;
	}

	if (!isSkill4Releasing) return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill4Montage) return;

	anim->PlaySkill4Montage();
	anim->Montage_JumpToSection("Charge_End", anim->Skill4Montage);
	isSkill4Releasing = false;

	GetWorld()->GetTimerManager().SetTimer
	(Skill4EndMotionDelayHandle, this, &UPlayerAttackComponent::Skill4EndMotionDelay, 1.0f, true);

	if (skill4FeverTime)
	{
		FTransform skillPosition = me->skillArrow->GetComponentTransform();
		GetWorld()->SpawnActor<APlayer1_Skill4>(skill4Factory, skillPosition);
	}
}

void UPlayerAttackComponent::Skill4FeverOnDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(Skill4FeverOnHandle);		//스킬4 피버타임 시작

	if (isSkillAttacking)
	{
		UE_LOG(LogTemp, Warning, TEXT("FeverOn"));
		skill4FeverTime = true;
		GetWorld()->GetTimerManager().SetTimer(Skill4FeverOffHandle, this, &UPlayerAttackComponent::Skill4FeverOffDelay, 2.0f, true);
	}
}

void UPlayerAttackComponent::Skill4FeverOffDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(Skill4FeverOffHandle);		//스킬4 피버타임 종료
	UE_LOG(LogTemp, Warning, TEXT("FeverOff"));
	skill4FeverTime = false;
}

void UPlayerAttackComponent::Skill4EndDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(Skill4EndDelayHandle);		//스킬4 애니메이션 종료

	if (!isSkillAttacking) return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill4Montage) return;

	anim->PlaySkill4Montage();
	anim->Montage_JumpToSection("Charge_End", anim->Skill4Montage);
	isSkillAttacking = false;

	GetWorld()->GetTimerManager().SetTimer
	(Skill4EndMotionDelayHandle, this, &UPlayerAttackComponent::Skill4EndMotionDelay, 1.0f, true);
}

void UPlayerAttackComponent::Skill4EndMotionDelay()
{
	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	GetWorld()->GetTimerManager().ClearTimer(Skill4EndMotionDelayHandle);		//스킬4 공격모션 종료
	anim->StopAllMontages(0.2f);
	isSkillAttacking = false;
}

void UPlayerAttackComponent::Skill4CanDodgeComp()
{
	isSkill4Flying = false;
	moveComp->SetMovementMode(EMovementMode::MOVE_Walking);		//애니메이션 z축 비활성화

	FTransform skillPosition = me->skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer1_Skill4Landing>(skill4LandingFactory, skillPosition);

	if (!isSkill4Releasing)
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
	--skill4CoolTime;

	UUI_ActionPlayer1* OwnerWidget = me->Widget;
	if (OwnerWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Widget"));
		return;
	}
	OwnerWidget->UpdateSkill4CoolTime();

	if (skill4CoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Skill4CoolTimerHandle);
		isCoolTimeSkill4 = false;						//스킬4 사용가능
	}
}

void UPlayerAttackComponent::InputUltimate()
{
	//구르기, 대쉬공격, 스킬공격, 쿨타임, 피격중에 사용X
	UPlayerMoveComponent* moveVar = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || isDashAttacking || isSkillAttacking || isCoolTimeUltimate || isImpacting)
		return;

	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->UltimateMontage) return;

	anim->PlayUltimateMontage();

	isUltimateAttacking = true;
	ultimateCoolTime = maxUltimateCoolTime;
	GetWorld()->GetTimerManager().SetTimer(UltimateCoolTimerHandle, this, &UPlayerAttackComponent::CoolDownUltimate, 1.0f, true);
	isCoolTimeUltimate = true;		//궁극기 쿨타임 on

	//충격체 컴포넌트 활성화
	me->ultimateBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->ultimateEffectComp->SetVisibility(true);
}

void UPlayerAttackComponent::CoolDownUltimate()
{
	--ultimateCoolTime;

	if (ultimateCoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(UltimateCoolTimerHandle);
		isCoolTimeUltimate = false;						//궁극기 사용가능
	}
}

void UPlayerAttackComponent::PlayerDie()
{
	auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
	if (!anim || !anim->DieMontage) return;
	
	anim->PlayDieMontage();
	isImpacting = true;
	GetWorld()->GetTimerManager().SetTimer(DieDelayTimerHandle, this, &UPlayerAttackComponent::GamePause, 2.0f, true);
}

void UPlayerAttackComponent::GamePause()
{
	GetWorld()->GetTimerManager().ClearTimer(DieDelayTimerHandle);
	APlayerController* const MyPlayer = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (MyPlayer != NULL)
	{
		MyPlayer->SetPause(true);
	}
}
