// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComponent.h"
#include "PlayerAttackComponent.h"
#include "Player1Anim.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UI_ActionPlayer1.h"

UPlayerMoveComponent::UPlayerMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	isRollingAnim = false;
	maxRollingCoolTime = 5;								//구르기 쿨타임 = 5초
	rollingCoolTime = 0;								//0초로 초기화
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
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMoveComponent::OutputRun);
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
	if (isRollingAnim || attackVar->isAttacking || attackVar->isSkillAttacking || attackVar->isSkill4Attacking
		|| attackVar->isUltimateAttacking || attackVar->isImpacting) return;

	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void UPlayerMoveComponent::InputRun()
{
	UPlayerAttackComponent* attackVar = GetOwner()->FindComponentByClass<UPlayerAttackComponent>();
	if (attackVar->isSkill2Attacking) return;			//스킬2사용중이면 적용 x

	auto movement = moveComp;
	movement->MaxWalkSpeed = runSpeed;
}

void UPlayerMoveComponent::OutputRun()
{
	UPlayerAttackComponent* attackVar = GetOwner()->FindComponentByClass<UPlayerAttackComponent>();
	if (attackVar->isSkill2Attacking) return;			//스킬2사용중이면 적용 x

	auto movement = moveComp;
	movement->MaxWalkSpeed = walkSpeed;
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
		UUI_ActionPlayer1* Widget = me->Widget;
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
			Widget->OffVisibilityChargeBar();
		}

		isRollingAnim = true;
		isCoolTimeRolling = true;		//구르기 쿨타임 on
		rollingCoolTime = maxRollingCoolTime;
		//쿨타임 돌리기
		GetWorld()->GetTimerManager().SetTimer(RollingCoolTimerHandle, this, &UPlayerMoveComponent::CoolDownRolling, 1.0f, true);
		Widget->VisibilityDodgeBar();
		Widget->UpdateDodgeCoolTime();
	}
}

void UPlayerMoveComponent::CoolDownRolling()
{
	--rollingCoolTime;

	UUI_ActionPlayer1* Widget = me->Widget;
	Widget->UpdateDodgeCoolTime();

	if (rollingCoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RollingCoolTimerHandle);
		isCoolTimeRolling = false;						//구르기 사용가능
		Widget->VisibilityDodgeBar();
	}
}

void UPlayerMoveComponent::DodgeEnd()
{
	isRollingAnim = false;
}
