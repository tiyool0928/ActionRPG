// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComponent.h"
#include "PlayerAttackComponent.h"
#include "Player1Anim.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

UPlayerMoveComponent::UPlayerMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	isRollingAnim = false;
	rollingCoolTime = 5;								//구르기 쿨타임 = 5초
	isCoolTimeRolling = false;
}

void UPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	//초기 속도 걷기속도
	moveComp->MaxWalkSpeed = walkSpeed;
}

void UPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move();					//캐릭터 이동 함수
}

void UPlayerMoveComponent::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	//좌우 축 바인딩
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMoveComponent::Turn);
	//상하 축 바인딩
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMoveComponent::LookUp);
	//좌우 입력 바인딩
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayerMoveComponent::InputHorizontal);
	//상하 입력 바인딩
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMoveComponent::InputVertical);
	//구르기 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("DodgeRoll"), IE_Pressed, this, &UPlayerMoveComponent::InputDodgeRoll);
	//달리기 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerMoveComponent::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMoveComponent::InputRun);
}

void UPlayerMoveComponent::Turn(float value)
{
	me->AddControllerYawInput(value);
}

void UPlayerMoveComponent::LookUp(float value)
{
	me->AddControllerPitchInput(value);
}

void UPlayerMoveComponent::InputHorizontal(float value)
{
	direction.Y = value;
}

void UPlayerMoveComponent::InputVertical(float value)
{
	direction.X = value;
}

void UPlayerMoveComponent::Move()
{
	//구르고있거나 공격중,피격중이면 이동 불가능
	UPlayerAttackComponent* attackVar = GetOwner()->FindComponentByClass<UPlayerAttackComponent>();
	if (isRollingAnim || attackVar->isAttacking || attackVar->isSkillAttacking
		|| attackVar->isUltimateAttacking || attackVar->isImpacting) return;

	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void UPlayerMoveComponent::InputRun()
{
	auto movement = moveComp;
	//달리기 모드상태일 때
	if (movement->MaxWalkSpeed > walkSpeed)
	{
		movement->MaxWalkSpeed = walkSpeed;
	}
	else
	{
		movement->MaxWalkSpeed = runSpeed;
	}
}

void UPlayerMoveComponent::InputDodgeRoll()
{
	//구르기 애니메이션 재생
	//구르기 쿨타임X, 무브먼트가 flyingX, 궁극기 모션중X, 피격중X 라면
	UPlayerAttackComponent* attackVar = GetOwner()->FindComponentByClass<UPlayerAttackComponent>();
	if (!isCoolTimeRolling && !attackVar->isSkill4Flying && !attackVar->isUltimateAttacking && !attackVar->isImpacting)
	{
		auto movement = moveComp;
		if (attackVar->isSkill2Attacking)		//스킬2 활성화중이었다면
		{
			movement->MaxWalkSpeed = walkSpeed;			//다시 걸음속도로 변환
			//충돌체 컴포넌트 비활성화
			me->skill2BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//이펙트 컴포넌트 안보이게
			me->skill2EffectComp->SetVisibility(false);
			attackVar->isSkill2Attacking = false;
		}
		auto anim = Cast<UPlayer1Anim>(me->GetMesh()->GetAnimInstance());
		anim->PlayDodgeRollAnim();		//구르기 애니메이션 on
		if (attackVar->isSkillAttacking || attackVar->isAttacking || attackVar->skill4FeverTime)
		{
			attackVar->isAttacking = false;			//공격 중 구르기 했을 경우 공격 bool타입 false로 변환
			attackVar->isSkillAttacking = false;
			attackVar->skill4FeverTime = false;
		}

		isRollingAnim = true;
		isCoolTimeRolling = true;		//구르기 쿨타임 on
		//쿨타임 돌리기
		GetWorld()->GetTimerManager().SetTimer(RollingCoolTimerHandle, this, &UPlayerMoveComponent::CoolDownRolling, 1.0f, true);
		GetWorld()->GetTimerManager().SetTimer(RollingAnimTimerHandle, this, &UPlayerMoveComponent::RollingDelay, 0.5f, true);
	}
}

void UPlayerMoveComponent::CoolDownRolling()
{
	--rollingCoolTime;

	if (rollingCoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RollingCoolTimerHandle);
		isCoolTimeRolling = false;						//구르기 사용가능
		rollingCoolTime = 5;
	}
}

void UPlayerMoveComponent::RollingDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(RollingAnimTimerHandle);
	isRollingAnim = false;						//구르기 애니메이션끝
}