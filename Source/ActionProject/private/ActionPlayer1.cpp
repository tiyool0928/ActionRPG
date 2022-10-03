// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayer1.h"
#include "Player1Anim.h"
#include "PlayerMoveComponent.h"
#include "PlayerAttackComponent.h"
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
#include "UI_ActionPlayer1.h"
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
	springArmComp->bDoCollisionTest = false;			//카메라 시야 가릴 때 자동 카메라 위치조정 비활성화
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
	ultimateBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	playerAttack = CreateDefaultSubobject<UPlayerAttackComponent>(TEXT("PlayerAttack"));

	player1MaxHealth = 1000;
	player1Health = player1MaxHealth;

	JumpMaxCount = 2;									//다중점프 설정
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

	UE_LOG(LogTemp, Warning, TEXT("Player1Health: %f"), player1Health);

	if (IsValid(HPBarWidget))
	{
		Widget = Cast<UUI_ActionPlayer1>(CreateWidget(GetWorld(), HPBarWidget));
		if (Widget != nullptr)
		{
			Widget->SetOwnerPlayer(this);
			Widget->AddToViewport();
			Widget->UpdateHealthBar();
		}
	}
}

// Called every frame
void AActionPlayer1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverLapSkill2Actors.Num() > 0)				//스킬2를 쓰는중에 액터가 들어와있으면
	{
		UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
		if (!attackVar->skill2Delay)							//딜레이가 비활성화 되어있다면
		{
			for (int i = 0; i < OverLapSkill2Actors.Num(); i++)
			{
				UGameplayStatics::ApplyDamage(OverLapSkill2Actors[i], 100.0f, nullptr, this, nullptr);
			}
			attackVar->skill2Delay = true;
			GetWorldTimerManager().SetTimer(Skill2DamageDelayHandle, this, &AActionPlayer1::Skill2DamageDelay, 0.5f, true);
		}
	}
}

// Called to bind functionality to input
void AActionPlayer1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	playerMove->SetupInputBinding(PlayerInputComponent);
	playerAttack->SetupInputBinding(PlayerInputComponent);
	//공격 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AActionPlayer1::LMB_Click);
}

void AActionPlayer1::LMB_Click()
{
	//UE_LOG(LogTemp, Warning, TEXT("LMB_Click!"));
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	if (!attackVar->isAttacking && !attackVar->isDashAttacking)			//노말공격도 대쉬공격도 하고 있지않은 상태
	{
		auto movement = GetCharacterMovement();		//달리는 중이면 대쉬공격 실행
		UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();
		

		if (movement->MaxWalkSpeed == moveVar->runSpeed)
			attackVar->DashAttack();
		else										//아니면 일반 공격
			attackVar->NormalAttack();
	}
	else if(attackVar->isAttacking)							//공격중이면
	{
		attackVar->isAttackButtonWhenAttack = true;
	}
}

void AActionPlayer1::AttackInputChecking()
{
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	if (attackVar->isAttackButtonWhenAttack)
	{
		attackVar->comboCnt++;
		if (attackVar->comboCnt >= 3)
			attackVar->comboCnt = 0;
		attackVar->isAttackButtonWhenAttack = false;
		attackVar->NormalAttack();
	}
}

void AActionPlayer1::EndAttacking()
{
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	attackVar->isAttacking = false;
	attackVar->isSkillAttacking = false;
	if (attackVar->isSkill2Attacking)		//스킬2 활성화중이었다면
	{
		auto movement = GetCharacterMovement();
		UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();

		movement->MaxWalkSpeed = moveVar->walkSpeed;			//다시 걸음속도로 변환
		//충돌체 컴포넌트 비활성화
		skill2BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//이펙트 컴포넌트 안보이게
		skill2EffectComp->SetVisibility(false);
		attackVar->isSkill2Attacking = false;
	}
	else if (attackVar->isUltimateAttacking)	//궁극기 사용한 상태라면
	{
		attackVar->isUltimateAttacking = false;
		ultimateBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ultimateEffectComp->SetVisibility(false);
	}
	attackVar->comboCnt = 0;
}

void AActionPlayer1::AttackDamageApplying()
{
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	attackVar->AttackDamageApplyingComp();
}

void AActionPlayer1::AttackDamageEnd()
{
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	attackVar->AttackDamageEndComp();
}

void AActionPlayer1::CreateSkill1Effect()
{
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	attackVar->CreateSkill1EffectComp();
}

void AActionPlayer1::Skill2DamageDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(Skill2DamageDelayHandle);		//스킬2 틱데미지 딜레이 초기화
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	attackVar->skill2Delay = false;
	UE_LOG(LogTemp, Warning, TEXT("Delay"));
}

void AActionPlayer1::CreateSkill3Effect()
{
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	attackVar->CreateSkill3EffectComp();
}

void AActionPlayer1::Skill4CanDodge()
{
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	attackVar->Skill4CanDodgeComp();
}

void AActionPlayer1::SmallImpactEnd()
{
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	attackVar->isImpacting = false;
}

void AActionPlayer1::CreateGhostTrail_Dodge()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	FRotator rotator = GetActorRotation();
	FVector  SpawnLocation = GetActorLocation();
	UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();
	auto GTrail_Dodge = Cast<AGhostTrail>(GetWorld()->SpawnActor<AGhostTrail>(moveVar->ghostTrailDodgeFactory, SpawnLocation, rotator, SpawnParams));
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
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	auto GTrail_Skill = Cast<AGhostTrail_Skill>(GetWorld()->SpawnActor<AGhostTrail_Skill>(attackVar->ghostTrailSkillFactory, SpawnLocation, rotator, SpawnParams));
	if (GTrail_Skill)
	{
		GTrail_Skill->Init(GetMesh());
	}
}

void AActionPlayer1::WeaponOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	if (!attackVar->isAttacking) return;
	if (OtherActor && (OtherActor != this) && OtherComp && attackVar->canDamage)
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
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();
	if (!attackVar->isUltimateAttacking) return;

	if (OtherActor && (OtherActor != this) && OtherComp && attackVar->canDamage)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 1000.0f, nullptr, this, nullptr);
	}
}

//피격 함수
float AActionPlayer1::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		player1Health -= Damage;
		if (player1Health < 0)
		{
			player1Health = 0;
		}
		Widget->UpdateHealthBar();
		//UE_LOG(LogTemp, Warning, TEXT("Player1Health: %f"), player1Health);
	}

	UPlayerMoveComponent* moveVar = this->FindComponentByClass<UPlayerMoveComponent>();
	UPlayerAttackComponent* attackVar = this->FindComponentByClass<UPlayerAttackComponent>();

	if (player1Health <= 0)
	{
		SetActorEnableCollision(false);
		attackVar->PlayerDie();
		UE_LOG(LogTemp, Warning, TEXT("Player1 Die"));
		return ActualDamage;
	}

	//스킬공격, 궁극기, 대쉬공격, 구르기 중에는 피격모션 X
	if (attackVar->isSkillAttacking || attackVar->isSkill2Attacking || attackVar->isUltimateAttacking
		|| attackVar->isDashAttacking || moveVar->isRollingAnim) return ActualDamage;

	auto anim = Cast<UPlayer1Anim>(GetMesh()->GetAnimInstance());
	anim->PlaySmallImpactMontage();
	
	attackVar->isImpacting = true;
	if (attackVar->isAttacking)					//일반공격중에 피격모션이 나왔을 경우
	{
		attackVar->isAttacking = false;			//공격중을 false로
	}
		

	return ActualDamage;
}