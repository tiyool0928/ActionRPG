// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1MoveComponent.h"
#include "Player1AttackComponent.h"
#include "Player1Anim.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UI_ActionPlayer1.h"

UPlayer1MoveComponent::UPlayer1MoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayer1MoveComponent::BeginPlay()
{
	Super::BeginPlay();

	//초기 속도 걷기속도
	moveComp->MaxWalkSpeed = walkSpeed;
}

void UPlayer1MoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move();					//캐릭터 이동 함수
}

void UPlayer1MoveComponent::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	//좌우 축 바인딩
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayer1MoveComponent::Turn);
	//상하 축 바인딩
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayer1MoveComponent::LookUp);
	//좌우 입력 바인딩
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayer1MoveComponent::InputHorizontal);
	//상하 입력 바인딩
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayer1MoveComponent::InputVertical);
	//달리기 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayer1MoveComponent::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayer1MoveComponent::OutputRun);
	//구르기 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("DodgeRoll"), IE_Pressed, this, &UPlayer1MoveComponent::InputDodgeRoll);
}

void UPlayer1MoveComponent::Turn(float value)
{
	me->AddControllerYawInput(value);
}

void UPlayer1MoveComponent::LookUp(float value)
{
	me->AddControllerPitchInput(value);
}

void UPlayer1MoveComponent::InputHorizontal(float value)
{
	direction.Y = value;
}

void UPlayer1MoveComponent::InputVertical(float value)
{
	direction.X = value;
}

void UPlayer1MoveComponent::Move()
{
	//구르고있거나 공격중,피격중이면 이동 불가능
	UPlayer1AttackComponent* attackVar = GetOwner()->FindComponentByClass<UPlayer1AttackComponent>();
	if (isRollingAnim || attackVar->isAttacking || attackVar->isSkillAttacking || attackVar->isSkill4Attacking
		|| attackVar->isUltimateAttacking || attackVar->isImpacting) return;

	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void UPlayer1MoveComponent::InputRun()
{
	UPlayer1AttackComponent* attackVar = GetOwner()->FindComponentByClass<UPlayer1AttackComponent>();
	if (attackVar->isSkill2Attacking) return;			//스킬2사용중이면 적용 x

	auto movement = moveComp;
	movement->MaxWalkSpeed = runSpeed;
}

void UPlayer1MoveComponent::OutputRun()
{
	UPlayer1AttackComponent* attackVar = GetOwner()->FindComponentByClass<UPlayer1AttackComponent>();
	if (attackVar->isSkill2Attacking) return;			//스킬2사용중이면 적용 x

	auto movement = moveComp;
	movement->MaxWalkSpeed = walkSpeed;
}

void UPlayer1MoveComponent::InputDodgeRoll()
{
	//구르기 애니메이션 재생
	//구르기 쿨타임X, 무브먼트가 flyingX, 궁극기 모션중X, 피격중X 라면
	UPlayer1MoveComponent* moveVar = me->FindComponentByClass<UPlayer1MoveComponent>();
	UPlayer1AttackComponent* attackVar = me->FindComponentByClass<UPlayer1AttackComponent>();
	if (!isCoolTimeRolling && !attackVar->isSkill4Flying && !attackVar->isUltimateAttacking && !attackVar->isImpacting)
	{
		auto movement = me->GetCharacterMovement();
		if (attackVar->isSkill2Attacking)		//스킬2 활성화중이었다면
		{
			movement->MaxWalkSpeed = moveVar->walkSpeed;			//다시 걸음속도로 변환
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

		if (attackVar->isSkill4Attacking)
		{
			attackVar->isSkill4Releasing = false;
			attackVar->isSkill4Attacking = false;
			me->Widget->OffVisibilityChargeBar();
		}

		isRollingAnim = true;
		isCoolTimeRolling = true;		//구르기 쿨타임 on
		rollingCoolTime = maxRollingCoolTime;
		//쿨타임 돌리기
		GetWorld()->GetTimerManager().SetTimer(RollingCoolTimerHandle, this, &UPlayer1MoveComponent::CoolDownRolling, 1.0f, true);
		me->Widget->VisibilityDodgeBar();
		me->Widget->UpdateDodgeCoolTime();
	}
}

void UPlayer1MoveComponent::CoolDownRolling()
{
	--rollingCoolTime;
	me->Widget->UpdateDodgeCoolTime();

	if (rollingCoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RollingCoolTimerHandle);
		isCoolTimeRolling = false;						//구르기 사용가능
		me->Widget->VisibilityDodgeBar();
	}
}

void UPlayer1MoveComponent::DodgeEnd()
{
	isRollingAnim = false;
}