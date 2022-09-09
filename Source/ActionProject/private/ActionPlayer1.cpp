// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayer1.h"
#include "Player1Anim.h"
#include "Player1_Skill1.h"
#include "Player1_Skill3.h"
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
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_healAura_2.P_ky_healAura_2'"));

	if (TempEffect.Succeeded())
	{
		skill2EffectComp->SetTemplate(TempEffect.Object);
		skill2EffectComp->SetRelativeLocation(FVector(0, 0, 15));
		skill2EffectComp->SetWorldScale3D(FVector(0.25f, 0.25f, 0.1f));
		skill2EffectComp->SetVisibility(false);
	}

	bUseControllerRotationYaw = true;					//클래스디폴트 Yaw 설정
	JumpMaxCount = 2;									//다중점프 설정
	isRollingAnim = false;
	rollingCoolTime = 5;								//구르기 쿨타임 = 5초
	isAttacking = false;
	isSkillAttacking = false;
	isSkill2Attacking = false;
	isAttackButtonWhenAttack = false;
	comboCnt = 0;										//처음 공격은 0번째콤보부터
	isCoolTimeRolling = false;
	canDamage = false;
	skill2Delay = false;
	skill4FeverTime = false;
}

// Called when the game starts or when spawned
void AActionPlayer1::BeginPlay()
{
	Super::BeginPlay();

	//초기 속도 걷기속도
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	//무기 충돌체에 overlapbegin 할당
	weaponBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AActionPlayer1::WeaponOnOverlapBegin);
	//스킬2 충돌체에 overlapbegin 할당
	skill2BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AActionPlayer1::Skill2OnOverlapBegin);
	skill2BoxComp->OnComponentEndOverlap.AddDynamic(this, &AActionPlayer1::Skill2OnOverlapEnd);
}

// Called every frame
void AActionPlayer1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();					//캐릭터 이동 함수

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

	//좌우 축 바인딩
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AActionPlayer1::Turn);
	//상하 축 바인딩
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AActionPlayer1::LookUp);
	//좌우 입력 바인딩
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AActionPlayer1::InputHorizontal);
	//상하 입력 바인딩
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AActionPlayer1::InputVertical);
	//점프 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("DodgeRoll"), IE_Pressed, this, &AActionPlayer1::InputDodgeRoll);
	//달리기 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &AActionPlayer1::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &AActionPlayer1::InputRun);
	//공격 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AActionPlayer1::LMB_Click);
	//스킬 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Skill1"), IE_Pressed, this, &AActionPlayer1::InputSkill1);
	PlayerInputComponent->BindAction(TEXT("Skill2"), IE_Pressed, this, &AActionPlayer1::InputSkill2);
	PlayerInputComponent->BindAction(TEXT("Skill3"), IE_Pressed, this, &AActionPlayer1::InputSkill3);
	PlayerInputComponent->BindAction(TEXT("Skill4"), IE_Pressed, this, &AActionPlayer1::InputSkill4);
}

void AActionPlayer1::Turn(float value)
{
	AddControllerYawInput(value);
}

void AActionPlayer1::LookUp(float value)
{
	AddControllerPitchInput(value);
}


void AActionPlayer1::InputHorizontal(float value)
{
	direction.Y = value;
}

void AActionPlayer1::InputVertical(float value)
{
	direction.X = value;
}

void AActionPlayer1::InputDodgeRoll()
{
	//구르기 애니메이션 재생
	if (!isCoolTimeRolling && !isDashAttacking)				//구르기 쿨타임이 돌고 있지 않으면서 대쉬공격중이 아니라면
	{
		auto movement = GetCharacterMovement();
		if (isSkill2Attacking)		//스킬2 활성화중이었다면
		{
			movement->MaxWalkSpeed = walkSpeed;			//다시 걸음속도로 변환
			//충돌체 컴포넌트 비활성화
			skill2BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//이펙트 컴포넌트 안보이게
			skill2EffectComp->SetVisibility(false);
			isSkill2Attacking = false;
		}
		auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
		anim->PlayDodgeRollAnim();		//구르기 애니메이션 on
		isAttacking = false;			//공격 중 구르기 했을 경우 공격 bool타입 false로 변환
		isSkillAttacking = false;
		isRollingAnim = true;
		isCoolTimeRolling = true;		//구르기 쿨타임 on
		//쿨타임 돌리기
		GetWorldTimerManager().SetTimer(RollingCoolTimerHandle, this, &AActionPlayer1::CoolDownRolling, 1.0f, true);
		GetWorldTimerManager().SetTimer(RollingAnimTimerHandle, this, &AActionPlayer1::RollingDelay, 0.5f, true);
	}
}

void AActionPlayer1::CoolDownRolling()
{
	--rollingCoolTime;
	
	if (rollingCoolTime <= 0)
	{
		GetWorldTimerManager().ClearTimer(RollingCoolTimerHandle);
		isCoolTimeRolling = false;						//구르기 사용가능
		rollingCoolTime = 5;
	}
}

void AActionPlayer1::RollingDelay()
{
	GetWorldTimerManager().ClearTimer(RollingAnimTimerHandle);
	isRollingAnim = false;						//구르기 애니메이션끝
}

void AActionPlayer1::Move()
{
	//구르고있거나 공격중이면 이동 불가능
	if (isRollingAnim || isAttacking || isSkillAttacking) return;

	direction = FTransform(GetControlRotation()).TransformVector(direction);
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void AActionPlayer1::InputRun()
{
	auto movement = GetCharacterMovement();
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

void AActionPlayer1::LMB_Click()
{
	//UE_LOG(LogTemp, Warning, TEXT("LMB_Click!"));
	if (!isAttacking && !isDashAttacking)			//노말공격도 대쉬공격도 하고 있지않은 상태
	{
		auto movement = GetCharacterMovement();		//달리는 중이면 대쉬공격 실행
		if (movement->MaxWalkSpeed == runSpeed)
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
	//구르고 있거나 이미 대쉬공격중 // 스킬공격중이면 공격X
	if (isRollingAnim || isDashAttacking || isSkillAttacking || isSkill2Attacking) return;

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
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);		//애니메이션 z축 비활성화
}

void AActionPlayer1::NormalAttack()
{
	if (isRollingAnim) return;			//구르고 있으면 공격X
	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();		//bp_player자체로 애니메이션 받는법
	//UE_LOG(LogTemp, Warning, TEXT("NormalAttack!"));
	if (!anim || !anim->NormalAttackMontage) return;
	const char* comboList[] = { "Attack1_Start", "Attack2_Start","Attack3_Start" };

	if (!anim->Montage_IsPlaying(anim->NormalAttackMontage))	//공격 몽타주가 실행중이지 않을때
	{
		anim->PlayNormalAttackAnim();		//일반공격 애니메이션 on
		isAttacking = true;
	}
	else														//공격몽타주가 실행중일때
	{
		anim->PlayNormalAttackAnim();
		anim->Montage_JumpToSection(FName(comboList[comboCnt]), anim->NormalAttackMontage);
		isAttacking = true;
	}
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
	auto movement = GetCharacterMovement();
	if (isSkill2Attacking)		//스킬2 활성화중이었다면
	{
		movement->MaxWalkSpeed = walkSpeed;			//다시 걸음속도로 변환
		//충돌체 컴포넌트 비활성화
		skill2BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//이펙트 컴포넌트 안보이게
		skill2EffectComp->SetVisibility(false);
		isSkill2Attacking = false;
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
}

void AActionPlayer1::InputSkill1()
{
	if (isRollingAnim || isDashAttacking || isSkillAttacking) return;			//구르기/대쉬공격/스킬공격중이면 공격X

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill1Montage) return;

	anim->PlaySkill1Montage();
	isSkillAttacking = true;
}

void AActionPlayer1::CreateSkill1Effect()
{
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer1_Skill1>(skill1Factory, skillPosition);
}

void AActionPlayer1::InputSkill2()
{
	if (isRollingAnim || isDashAttacking || isSkillAttacking) return;			//구르기/대쉬공격/스킬공격중이면 공격X

	if(isAttacking)							//공격중에 스킬2를 썼으면
		isAttacking = false;

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill2Montage) return;

	anim->PlaySkill2Montage();
	isSkill2Attacking = true;
	auto movement = GetCharacterMovement();
	movement->MaxWalkSpeed = runSpeed;
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

void AActionPlayer1::InputSkill3()
{
	if (isRollingAnim || isDashAttacking || isSkillAttacking) return;			//구르기/대쉬공격/스킬공격중이면 공격X

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill3Montage) return;

	anim->PlaySkill3Montage();
	isSkillAttacking = true;
}

void AActionPlayer1::CreateSkill3Effect()
{
	UE_LOG(LogTemp, Warning, TEXT("Before Effect"));
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer1_Skill3>(skill3Factory, skillPosition);
}

void AActionPlayer1::InputSkill4()
{
	if (isRollingAnim || isDashAttacking || isSkillAttacking) return;			//구르기/대쉬공격/스킬공격중이면 공격X

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill4Montage) return;

	anim->PlaySkill4Montage();
	anim->Montage_JumpToSection("Section_Start", anim->Skill4Montage);
	isSkillAttacking = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);		//애니메이션 z축 활성화
	GetWorldTimerManager().SetTimer(Skill4FeverOnHandle, this, &AActionPlayer1::Skill4FeverOnDelay, 2.0f, true);
}

void AActionPlayer1::Skill4FeverOnDelay()
{
	GetWorldTimerManager().ClearTimer(Skill4FeverOnHandle);		//스킬4 피버타임 시작
	UE_LOG(LogTemp, Warning, TEXT("FeverOn"));
	skill4FeverTime = true;
	GetWorldTimerManager().SetTimer(Skill4FeverOffHandle, this, &AActionPlayer1::Skill4FeverOffDelay, 2.0f, true);
}

void AActionPlayer1::Skill4FeverOffDelay()
{
	GetWorldTimerManager().ClearTimer(Skill4FeverOffHandle);		//스킬4 피버타임 종료
	UE_LOG(LogTemp, Warning, TEXT("FeverOff"));
	skill4FeverTime = false;
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

