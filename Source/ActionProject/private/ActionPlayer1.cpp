// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayer1.h"
#include "Player1Anim.h"
#include "PlayerMoveComponent.h"
#include "Player1_Skill1.h"
#include "Player1_Skill3.h"
#include "Player1_Skill4.h"
#include "Player1_Skill4Landing.h"
#include "Player1_UltimateBoom.h"
#include "GhostTrail.h"
#include "GhostTrail_Skill.h"
#include "EnemyLog.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
AActionPlayer1::AActionPlayer1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스켈레탈 메시 데이터
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		//컴포넌트 위치 (0, 0, -90) 회전 (0, -90, 0)
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	//카메라 설정
	//SpringArm 컴포넌트
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 0, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;		//암컴포넌트 폰 제어
	//camera 컴포넌트
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;		//카메라 폰 제어
	//무기mesh 컴포넌트
	weaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComp"));
	weaponComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempWeaponMesh(TEXT("StaticMesh'/Game/Mesh/BusterSword/SM_BusterSword.SM_BusterSword'"));
	if (TempWeaponMesh.Succeeded())
	{
		weaponComp->SetStaticMesh(TempWeaponMesh.Object);
		weaponComp->SetRelativeLocation(FVector(-63, -6, 62));
		weaponComp->SetRelativeRotation(FRotator(160, 10, 0));
	}
	//스킬arrow 컴포넌트
	skillArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("skillArrow"));
	skillArrow->SetupAttachment(RootComponent);
	skillArrow->SetRelativeLocation(FVector(70, 0, 0));

	weaponBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Collision"));	//충돌체 컴포넌트
	weaponBoxComp->SetupAttachment(weaponComp);
	weaponBoxComp->SetRelativeLocation(FVector(-30, 0, -18));
	weaponBoxComp->SetRelativeRotation(FRotator(-30, 0, 0));
	weaponBoxComp->SetWorldScale3D(FVector(2, 0.5, 0.5));
	//스킬 2 충격체 컴포넌트
	skill2BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Skill2 Collision"));
	skill2BoxComp->SetupAttachment(RootComponent);
	skill2BoxComp->SetWorldScale3D(FVector(7, 7, 2));
	skill2BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//스킬 2 이펙트 컴포넌트
	skill2EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Skill2 Effect"));
	skill2EffectComp->SetupAttachment(skill2BoxComp);
	skill2EffectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> skill2Effect(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_healAura_2.P_ky_healAura_2'"));
	if (skill2Effect.Succeeded())
	{
		skill2EffectComp->SetTemplate(skill2Effect.Object);
		skill2EffectComp->SetRelativeLocation(FVector(0, 0, 15));
		skill2EffectComp->SetWorldScale3D(FVector(0.25f, 0.25f, 0.1f));
		skill2EffectComp->SetVisibility(false);
	}

	//궁극기 충격체 컴포넌트
	ultimateBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("ultimate Collision"));
	ultimateBoxComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ultimateBoxComp->SetRelativeLocation(FVector(-160, -27, 180));
	ultimateBoxComp->SetRelativeRotation(FRotator(40, 10, 0));
	ultimateBoxComp->SetWorldScale3D(FVector(1, 1, 6));
	//궁극기 이펙트 컴포넌트
	ultimateEffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ultimate Effect"));
	ultimateEffectComp->SetupAttachment(ultimateBoxComp);
	ultimateEffectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> ultimateEffect(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_fireStorm.P_ky_fireStorm'"));
	if (ultimateEffect.Succeeded())
	{
		ultimateEffectComp->SetTemplate(ultimateEffect.Object);
		ultimateEffectComp->SetVisibility(false);
		ultimateEffectComp->SetRelativeLocation(FVector(0, 0, -35));
		ultimateEffectComp->SetWorldScale3D(FVector(0.4f, 0.1f, 0.2f));
	}

	bUseControllerRotationYaw = true;					//클래스디폴트 Yaw 설정

	playerMove = CreateDefaultSubobject<UPlayerMoveComponent>(TEXT("PlayerMove"));

	player1MaxHealth = 1000;
	player1Health = player1MaxHealth;

	JumpMaxCount = 2;									//다중점프 설정
	isAttacking = false;
	isSkillAttacking = false;
	isSkill2Attacking = false;
	isSkill4Flying = false;
	isSkill4Releasing = false;
	isUltimateAttacking = false;
	isAttackButtonWhenAttack = false;
	isCoolTimeSkill1 = false;
	skill1CoolTime = 4;									//스킬1 쿨타임 = 4초
	isCoolTimeSkill2 = false;
	skill2CoolTime = 10;								//스킬2 쿨타임 = 10초
	isCoolTimeSkill3 = false;
	skill3CoolTime = 8;									//스킬3 쿨타임 = 8초
	isCoolTimeSkill4 = false;
	skill4CoolTime = 15;								//스킬4 쿨타임 = 15초
	isCoolTimeUltimate = false;
	ultimateCoolTime = 60;								//궁극기 쿨타임 = 60초
	comboCnt = 0;										//처음 공격은 0번째콤보부터
	canDamage = false;
	skill2Delay = false;
	skill4FeverTime = false;
}

// Called when the game starts or when spawned
void AActionPlayer1::BeginPlay()
{
	Super::BeginPlay();

	//무기 충돌체에 overlapbegin 할당
	weaponBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AActionPlayer1::WeaponOnOverlapBegin);
	//스킬2 충돌체에 overlapbegin 할당
	skill2BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AActionPlayer1::Skill2OnOverlapBegin);
	skill2BoxComp->OnComponentEndOverlap.AddDynamic(this, &AActionPlayer1::Skill2OnOverlapEnd);
	//궁극기 충돌체에 overlapbegin 할당
	ultimateBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AActionPlayer1::UltimateSmashOnOverlapBegin);
}

// Called every frame
void AActionPlayer1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverLapSkill2Actors.Num() > 0)				//스킬2를 쓰는중에 액터가 들어와있으면
	{
		if (!skill2Delay)							//딜레이가 비활성화 되어있다면
		{
			for (int i = 0; i < OverLapSkill2Actors.Num(); i++)
			{
				UGameplayStatics::ApplyDamage(OverLapSkill2Actors[i], 100.0f, nullptr, this, nullptr);
			}
			skill2Delay = true;
			GetWorldTimerManager().SetTimer(Skill2DamageDelayHandle, this, &AActionPlayer1::Skill2DamageDelay, 0.5f, true);
		}
	}
}

// Called to bind functionality to input
void AActionPlayer1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	playerMove->SetupInputBinding(PlayerInputComponent);
	//공격 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AActionPlayer1::LMB_Click);
	//스킬 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Skill1"), IE_Pressed, this, &AActionPlayer1::InputSkill1);
	PlayerInputComponent->BindAction(TEXT("Skill2"), IE_Pressed, this, &AActionPlayer1::InputSkill2);
	PlayerInputComponent->BindAction(TEXT("Skill3"), IE_Pressed, this, &AActionPlayer1::InputSkill3);
	PlayerInputComponent->BindAction(TEXT("Skill4"), IE_Pressed, this, &AActionPlayer1::InputSkill4);
	PlayerInputComponent->BindAction(TEXT("Skill4"), IE_Released, this, &AActionPlayer1::OutputSkill4);
	PlayerInputComponent->BindAction(TEXT("Ultimate"), IE_Pressed, this, &AActionPlayer1::InputUltimate);
}

void AActionPlayer1::LMB_Click()
{
	//UE_LOG(LogTemp, Warning, TEXT("LMB_Click!"));
	if (!isAttacking && !isDashAttacking)			//노말공격도 대쉬공격도 하고 있지않은 상태
	{
		auto movement = GetCharacterMovement();		//달리는 중이면 대쉬공격 실행
		UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();

		if (movement->MaxWalkSpeed == moveVar->runSpeed)
			DashAttack();
		else										//아니면 일반 공격
			NormalAttack();
	}
	else if(isAttacking)							//공격중이면
	{
		isAttackButtonWhenAttack = true;
	}
}

void AActionPlayer1::DashAttack()
{
	UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();
	//구르고 있거나 이미 대쉬공격중 // 스킬공격중이면 공격X
	if (moveVar->isRollingAnim || isDashAttacking || isSkillAttacking || isSkill2Attacking || isUltimateAttacking)
		return;

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->DashAttackMontage) return;

	anim->PlayDashAttackMontage();
	anim->Montage_JumpToSection("Section_Start", anim->DashAttackMontage);
	isAttacking = true;
	isAttacking = true;
	isDashAttacking = true;
	//1초 뒤 대쉬어택 모션 끝
	GetWorldTimerManager().SetTimer(DashAttackAnimTimerHandle, this, &AActionPlayer1::DashAttackDelay, 1.0f, true);
	
}

void AActionPlayer1::DashAttackDelay()
{
	GetWorldTimerManager().ClearTimer(DashAttackAnimTimerHandle);		//대쉬어택 모션타이머 초기화
	isAttacking = false;						//공격 끝
	isDashAttacking = false;					//대쉬공격 끝
}

void AActionPlayer1::NormalAttack()
{
	UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();
	//구르고 있으면 스킬, 궁극기 사용중이면 공격X
	if (moveVar->isRollingAnim || isSkillAttacking || isUltimateAttacking || isSkill4Flying) return;

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
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

void AActionPlayer1::AttackInputChecking()
{
	if (isAttackButtonWhenAttack)
	{
		comboCnt++;
		if (comboCnt >= 3)
			comboCnt = 0;
		isAttackButtonWhenAttack = false;
		NormalAttack();
	}
}

void AActionPlayer1::EndAttacking()
{
	isAttacking = false;
	isSkillAttacking = false;
	if (isSkill2Attacking)		//스킬2 활성화중이었다면
	{
		auto movement = GetCharacterMovement();
		UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();

		movement->MaxWalkSpeed = moveVar->walkSpeed;			//다시 걸음속도로 변환
		//충돌체 컴포넌트 비활성화
		skill2BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//이펙트 컴포넌트 안보이게
		skill2EffectComp->SetVisibility(false);
		isSkill2Attacking = false;
	}
	else if (isUltimateAttacking)	//궁극기 사용한 상태라면
	{
		isUltimateAttacking = false;
		ultimateBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ultimateEffectComp->SetVisibility(false);
	}
	comboCnt = 0;
}

void AActionPlayer1::AttackDamageApplying()
{
	canDamage = true;
}

void AActionPlayer1::AttackDamageEnd()
{
	canDamage = false;

	if (isUltimateAttacking)
	{
		FTransform skillPosition = skillArrow->GetComponentTransform();
		GetWorld()->SpawnActor<APlayer1_UltimateBoom>(ultimateFactory, skillPosition);
	}
}

void AActionPlayer1::InputSkill1()
{
	//구르기, 대쉬공격, 스킬공격, 궁극기, 쿨타임 중에 사용X
	UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || isDashAttacking || isSkillAttacking
		|| isSkill2Attacking || isUltimateAttacking || isCoolTimeSkill1)
		return;			

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill1Montage) return;

	anim->PlaySkill1Montage();
	isSkillAttacking = true;
	GetWorldTimerManager().SetTimer(Skill1CoolTimerHandle, this, &AActionPlayer1::CoolDownSkill1, 1.0f, true);
	isCoolTimeSkill1 = true;		//스킬1 쿨타임 on
}

void AActionPlayer1::CreateSkill1Effect()
{
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer1_Skill1>(skill1Factory, skillPosition);
}

void AActionPlayer1::CoolDownSkill1()
{
	--skill1CoolTime;

	if (skill1CoolTime <= 0)
	{
		GetWorldTimerManager().ClearTimer(Skill1CoolTimerHandle);
		isCoolTimeSkill1 = false;					//스킬1 사용가능
		skill1CoolTime = 4;							//쿨타임 초기화
	}
}

void AActionPlayer1::InputSkill2()
{
	//구르기, 대쉬공격, 스킬공격, 궁극기, 쿨타임 중에 사용X
	UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || isDashAttacking || isSkillAttacking
		|| isSkill2Attacking || isUltimateAttacking || isCoolTimeSkill2)
		return;

	if(isAttacking)							//공격중에 스킬2를 썼으면
		isAttacking = false;

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill2Montage) return;

	anim->PlaySkill2Montage();
	isSkill2Attacking = true;
	GetWorldTimerManager().SetTimer(Skill2CoolTimerHandle, this, &AActionPlayer1::CoolDownSkill2, 1.0f, true);
	isCoolTimeSkill2 = true;		//스킬2 쿨타임 on

	auto movement = GetCharacterMovement();
	movement->MaxWalkSpeed = moveVar->runSpeed;
	//충격체 컴포넌트 활성화
	skill2BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	skill2EffectComp->SetVisibility(true);
}

void AActionPlayer1::Skill2DamageDelay()
{
	GetWorldTimerManager().ClearTimer(Skill2DamageDelayHandle);		//스킬2 틱데미지 딜레이 초기화
	skill2Delay = false;
	UE_LOG(LogTemp, Warning, TEXT("Delay"));
}

void AActionPlayer1::CoolDownSkill2()
{
	--skill2CoolTime;

	if (skill2CoolTime <= 0)
	{
		GetWorldTimerManager().ClearTimer(Skill2CoolTimerHandle);
		isCoolTimeSkill2 = false;						//스킬2 사용가능
		skill2CoolTime = 10;							//쿨타임 초기화
	}
}

void AActionPlayer1::InputSkill3()
{
	//구르기, 대쉬공격, 스킬공격, 궁극기, 쿨타임 중에 사용X
	UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || isDashAttacking || isSkillAttacking
		|| isSkill2Attacking || isUltimateAttacking || isCoolTimeSkill3)
		return;			

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill3Montage) return;

	anim->PlaySkill3Montage();
	isSkillAttacking = true;
	GetWorldTimerManager().SetTimer(Skill3CoolTimerHandle, this, &AActionPlayer1::CoolDownSkill3, 1.0f, true);
	isCoolTimeSkill3 = true;		//스킬3 쿨타임 on
}

void AActionPlayer1::CreateSkill3Effect()
{
	UE_LOG(LogTemp, Warning, TEXT("Before Effect"));
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer1_Skill3>(skill3Factory, skillPosition);
}

void AActionPlayer1::CoolDownSkill3()
{
	--skill3CoolTime;

	if (skill3CoolTime <= 0)
	{
		GetWorldTimerManager().ClearTimer(Skill3CoolTimerHandle);
		isCoolTimeSkill3 = false;					//스킬3 사용가능
		skill3CoolTime = 8;							//쿨타임 초기화
	}
}

void AActionPlayer1::InputSkill4()
{
	//구르기, 대쉬공격, 스킬공격, 궁극기, 쿨타임 중에 사용X
	UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || isDashAttacking || isSkillAttacking
		|| isSkill2Attacking || isUltimateAttacking || isCoolTimeSkill4)
		return;			

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill4Montage) return;

	anim->PlaySkill4Montage();
	anim->Montage_JumpToSection("Section_Start", anim->Skill4Montage);
	isSkill4Releasing = true;
	isSkillAttacking = true;
	isSkill4Flying = true;
	GetWorldTimerManager().SetTimer(Skill4CoolTimerHandle, this, &AActionPlayer1::CoolDownSkill4, 1.0f, true);
	isCoolTimeSkill4 = true;		//스킬4 쿨타임 on

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);		//애니메이션 z축 활성화
	
	GetWorldTimerManager().SetTimer(Skill4FeverOnHandle, this, &AActionPlayer1::Skill4FeverOnDelay, 2.0f, true);
	GetWorldTimerManager().SetTimer(Skill4EndDelayHandle, this, &AActionPlayer1::Skill4EndDelay, 4.0f, true);
}

void AActionPlayer1::OutputSkill4()
{
	if (isSkill4Flying)
	{
		isSkill4Releasing = false;
	}

	if (!isSkill4Releasing) return;

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill4Montage) return;

	anim->PlaySkill4Montage();
	anim->Montage_JumpToSection("Charge_End", anim->Skill4Montage);
	isSkill4Releasing = false;

	GetWorldTimerManager().SetTimer
	(Skill4EndMotionDelayHandle, this, &AActionPlayer1::Skill4EndMotionDelay, 1.0f, true);
	
	if (skill4FeverTime)
	{	
		FTransform skillPosition = skillArrow->GetComponentTransform();
		GetWorld()->SpawnActor<APlayer1_Skill4>(skill4Factory, skillPosition);
	}
}

void AActionPlayer1::Skill4FeverOnDelay()
{
	GetWorldTimerManager().ClearTimer(Skill4FeverOnHandle);		//스킬4 피버타임 시작

	if (isSkillAttacking)
	{
		UE_LOG(LogTemp, Warning, TEXT("FeverOn"));
		skill4FeverTime = true;
		GetWorldTimerManager().SetTimer(Skill4FeverOffHandle, this, &AActionPlayer1::Skill4FeverOffDelay, 2.0f, true);
	}
}

void AActionPlayer1::Skill4FeverOffDelay()
{
	GetWorldTimerManager().ClearTimer(Skill4FeverOffHandle);		//스킬4 피버타임 종료
	UE_LOG(LogTemp, Warning, TEXT("FeverOff"));
	skill4FeverTime = false;
}

void AActionPlayer1::Skill4EndDelay()
{
	GetWorldTimerManager().ClearTimer(Skill4EndDelayHandle);		//스킬4 애니메이션 종료

	if (!isSkillAttacking) return;

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill4Montage) return;

	anim->PlaySkill4Montage();
	anim->Montage_JumpToSection("Charge_End", anim->Skill4Montage);
	isSkillAttacking = false;

	GetWorldTimerManager().SetTimer
	(Skill4EndMotionDelayHandle, this, &AActionPlayer1::Skill4EndMotionDelay, 1.0f, true);
}

void AActionPlayer1::Skill4EndMotionDelay()
{
	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	GetWorldTimerManager().ClearTimer(Skill4EndMotionDelayHandle);		//스킬4 공격모션 종료
	anim->StopAllMontages(0.2f);
	isSkillAttacking = false;
}

void AActionPlayer1::Skill4CanDodge()
{
	isSkill4Flying = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);		//애니메이션 z축 비활성화

	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer1_Skill4Landing>(skill4LandingFactory, skillPosition);

	if (!isSkill4Releasing)
	{
		auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
		GetWorldTimerManager().ClearTimer(Skill4EndDelayHandle);		//스킬4 애니메이션 종료
		anim->PlaySkill4Montage();
		anim->Montage_JumpToSection("Charge_End", anim->Skill4Montage);

		GetWorldTimerManager().SetTimer
		(Skill4EndMotionDelayHandle, this, &AActionPlayer1::Skill4EndMotionDelay, 1.0f, true);
	}
}

void AActionPlayer1::CoolDownSkill4()
{
	--skill4CoolTime;

	if (skill4CoolTime <= 0)
	{
		GetWorldTimerManager().ClearTimer(Skill4CoolTimerHandle);
		isCoolTimeSkill4 = false;						//스킬4 사용가능
		skill4CoolTime = 15;							//쿨타임 초기화
	}
}

void AActionPlayer1::InputUltimate()
{
	//구르기, 대쉬공격, 스킬공격, 쿨타임 중에 사용X
	UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();
	if (moveVar->isRollingAnim || isDashAttacking || isSkillAttacking || isCoolTimeUltimate)
		return;

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->UltimateMontage) return;

	anim->PlayUltimateMontage();

	isUltimateAttacking = true;
	GetWorldTimerManager().SetTimer(UltimateCoolTimerHandle, this, &AActionPlayer1::CoolDownUltimate, 1.0f, true);
	isCoolTimeUltimate = true;		//궁극기 쿨타임 on

	//충격체 컴포넌트 활성화
	ultimateBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ultimateEffectComp->SetVisibility(true);
}

void AActionPlayer1::CoolDownUltimate()
{
	--ultimateCoolTime;

	if (ultimateCoolTime <= 0)
	{
		GetWorldTimerManager().ClearTimer(UltimateCoolTimerHandle);
		isCoolTimeUltimate = false;						//궁극기 사용가능
		ultimateCoolTime = 60;							//쿨타임 초기화
	}
}

void AActionPlayer1::CreateGhostTrail_Dodge()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	FRotator rotator = GetActorRotation();
	FVector  SpawnLocation = GetActorLocation();
	auto GTrail_Dodge = Cast<AGhostTrail>(GetWorld()->SpawnActor<AGhostTrail>(ghostTrailDodgeFactory, SpawnLocation, rotator, SpawnParams));
	if (GTrail_Dodge)
	{
		GTrail_Dodge->Init(GetMesh());
	}
}

void AActionPlayer1::CreateGhostTrail_Skill()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	FRotator rotator = GetActorRotation();
	FVector  SpawnLocation = GetActorLocation();
	auto GTrail_Skill = Cast<AGhostTrail_Skill>(GetWorld()->SpawnActor<AGhostTrail_Skill>(ghostTrailSkillFactory, SpawnLocation, rotator, SpawnParams));
	if (GTrail_Skill)
	{
		GTrail_Skill->Init(GetMesh());
	}
}

void AActionPlayer1::WeaponOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isAttacking) return;
	if (OtherActor && (OtherActor != this) && OtherComp && canDamage)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 50.0f, nullptr, this, nullptr);
	}
}

void AActionPlayer1::Skill2OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		//AddUnique = 컨테이너에 없을 경우 추가
		OverLapSkill2Actors.AddUnique(OtherActor);			//skill2를 맞고있는 액터배열에 추가
		//UGameplayStatics::ApplyDamage(OtherActor, 100.0f, nullptr, this, nullptr);
	}
}


void AActionPlayer1::Skill2OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverLapSkill2Actors.Remove(OtherActor);
}

void AActionPlayer1::UltimateSmashOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isUltimateAttacking) return;

	if (OtherActor && (OtherActor != this) && OtherComp && canDamage)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 50.0f, nullptr, this, nullptr);
	}
}

