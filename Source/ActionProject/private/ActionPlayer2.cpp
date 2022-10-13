// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayer2.h"
#include "Player2Anim.h"
#include "GhostTrail.h"
#include "Player2_NormalAttack.h"
#include "Components/ArrowComponent.h"
#include <GameFramework/CharacterMovementComponent.h>

AActionPlayer2::AActionPlayer2()
{
	PrimaryActorTick.bCanEverTick = true;

	//무기mesh 컴포넌트
	weaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComp"));
	weaponComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempWeaponMesh(TEXT("StaticMesh'/Game/Mesh/MagicWand/SM_MagicWand.SM_MagicWand'"));
	if (TempWeaponMesh.Succeeded())
	{
		weaponComp->SetStaticMesh(TempWeaponMesh.Object);
		weaponComp->SetRelativeLocation(FVector(-10, 5, 5));
		weaponComp->SetRelativeRotation(FRotator(0, -3, -10));
		weaponComp->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	}

	//모자mesh 컴포넌트
	hatComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HatMeshComp"));
	hatComp->SetupAttachment(GetMesh(), TEXT("head_Socket"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempHatMesh(TEXT("StaticMesh'/Game/Mesh/MagicHat/SM_MagicHat.SM_MagicHat'"));
	if (TempHatMesh.Succeeded())
	{
		hatComp->SetStaticMesh(TempHatMesh.Object);
		hatComp->SetRelativeLocation(FVector(9, 0, 0));
		hatComp->SetRelativeRotation(FRotator(-80, 80, -90));
		hatComp->SetWorldScale3D(FVector(0.3f, 0.3f, 0.2f));
	}

	//스킬arrow 컴포넌트
	skillArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("skillArrow"));
	skillArrow->SetupAttachment(RootComponent);
	skillArrow->SetRelativeLocation(FVector(70, 0, 0));
}

void AActionPlayer2::BeginPlay()
{
	Super::BeginPlay();

	//초기 속도 걷기속도
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void AActionPlayer2::Tick(float DeltaTime)
{
	Move();					//캐릭터 이동 함수
}

void AActionPlayer2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//좌우 축 바인딩
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AActionPlayer2::Turn);
	//상하 축 바인딩
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AActionPlayer2::LookUp);
	//좌우 입력 바인딩
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AActionPlayer2::InputHorizontal);
	//상하 입력 바인딩
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AActionPlayer2::InputVertical);
	//달리기 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &AActionPlayer2::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &AActionPlayer2::OutputRun);
	//구르기 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("DodgeRoll"), IE_Pressed, this, &AActionPlayer2::InputDodgeRoll);

	//공격 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AActionPlayer2::LMB_Click);
}

void AActionPlayer2::LMB_Click()
{
	//UE_LOG(LogTemp, Warning, TEXT("LMB_Click!"));
	if (!isAttacking)			//노말공격도 대쉬공격도 하고 있지않은 상태
	{
		//auto movement = GetCharacterMovement();		//달리는 중이면 대쉬공격 실행
		//if (movement->MaxWalkSpeed == moveVar->runSpeed)
		//	DashAttack();
		//else										//아니면 일반 공격
		NormalAttack();
	}
}

void AActionPlayer2::NormalAttack()
{
	//구르고 있으면, 공격중이면 공격X
	if (isRollingAnim || isAttacking) return;

	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	anim->PlayNormalAttackAnim();		//일반공격 애니메이션 on
	isAttacking = true;
}

void AActionPlayer2::Turn(float value)
{
	AddControllerYawInput(value);
}

void AActionPlayer2::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void AActionPlayer2::InputHorizontal(float value)
{
	direction.Y = value;
}

void AActionPlayer2::InputVertical(float value)
{
	direction.X = value;
}

void AActionPlayer2::Move()
{
	//공격중이면
	if (isAttacking) return;

	direction = FTransform(GetControlRotation()).TransformVector(direction);
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void AActionPlayer2::InputRun()
{
	GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}

void AActionPlayer2::OutputRun()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void AActionPlayer2::InputDodgeRoll()
{
	if (isRollingAnim) return;
	//구르기 애니메이션 재생
	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());

	anim->PlayDodgeRollAnim();		//구르기 애니메이션 on
	//공격중이었으면
	if (isAttacking)
		isAttacking = false;

	isRollingAnim = true;
	isCoolTimeRolling = true;		//구르기 쿨타임 on
	rollingCoolTime = maxRollingCoolTime;
	//쿨타임 돌리기
	GetWorld()->GetTimerManager().SetTimer(RollingCoolTimerHandle, this, &AActionPlayer2::CoolDownRolling, 1.0f, true);
}

void AActionPlayer2::CoolDownRolling()
{
	--rollingCoolTime;

	if (rollingCoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RollingCoolTimerHandle);
		isCoolTimeRolling = false;						//구르기 사용가능
	}
}

void AActionPlayer2::DodgeEnd()
{
	isRollingAnim = false;
}

void AActionPlayer2::CreateGhostTrail_Dodge()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	FRotator rotator = GetActorRotation();
	FVector SpawnLocation = GetActorLocation();
	auto GTrail_Dodge = Cast<AGhostTrail>(GetWorld()->SpawnActor<AGhostTrail>(ghostTrailDodgeFactory, SpawnLocation, rotator, SpawnParams));
	if (GTrail_Dodge)
	{
		GTrail_Dodge->Init(GetMesh());
	}
}

void AActionPlayer2::Notify_DodgeEnd()
{
	DodgeEnd();
}

void AActionPlayer2::EndAttacking()
{
	isAttacking = false;
}

void AActionPlayer2::CreateNormalAttackEffect()
{
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_NormalAttack>(normalAttackFactory, skillPosition);
}
