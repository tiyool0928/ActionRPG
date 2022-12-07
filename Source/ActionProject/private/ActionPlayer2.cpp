// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayer2.h"
#include "Player2Anim.h"
#include "GhostTrail.h"
#include "Player2_NormalAttack.h"
#include "Player2_DashAttack.h"
#include "Player2_Skill1.h"
#include "Player2_Skill2_Portal.h"
#include "Player2_Skill3.h"
#include "Player2_Skill4Factory.h"
#include "player2_UltimateFactory.h"
#include "UI_ActionPlayer2.h"
#include "Components/ArrowComponent.h"
#include "Components/DecalComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>

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

	//라인트레이스arrow 컴포넌트
	lineTracingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("lineTracingArrow"));
	lineTracingArrow->SetupAttachment(RootComponent);
	lineTracingArrow->SetRelativeLocation(FVector(45, 0, 50));
	lineTracingArrow->SetRelativeRotation(FRotator(-10, 0, 0));

	skill2Area = CreateDefaultSubobject<UDecalComponent>(TEXT("skill2Area"));
	skill2Area->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UMaterialInstance> TempSkill2AreaDecal(TEXT("MaterialInstanceConstant'/Game/Megascans/Decals/Painted_Road_Marking_ui5ideecw/MI_Painted_Road_Marking_ui5ideecw_2K.MI_Painted_Road_Marking_ui5ideecw_2K'"));
	if (TempSkill2AreaDecal.Succeeded())
	{
		skill2Area->SetDecalMaterial(TempSkill2AreaDecal.Object);
		skill2Area->SetWorldScale3D(FVector(0.25f, 2, 2));
		skill2Area->SetVisibility(false);
	}
}

void AActionPlayer2::BeginPlay()
{
	Super::BeginPlay();

	//초기 속도 걷기속도
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	if (IsValid(HPBarWidget))
	{
		Widget = Cast<UUI_ActionPlayer2>(CreateWidget(GetWorld(), HPBarWidget));
		if (Widget != nullptr)
		{
			Widget->SetOwnerPlayer(this);
			Widget->AddToViewport();
			Widget->UpdateHealthBar();
		}
	}
}

void AActionPlayer2::Tick(float DeltaTime)
{
	Move();					//캐릭터 이동 함수

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	if (turnskill2Area)		//스킬2범위 표현이 켜진상태이면
	{
		FVector start = lineTracingArrow->GetComponentLocation();		
		FVector end = start + lineTracingArrow->GetComponentRotation().Vector() * 1500.0f;

		//lineTracingArrow 방향으로 1500까지 라인트레이스를 쏴준다.
		if (GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECollisionChannel::ECC_WorldDynamic, params))
		{
			UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR : %s"), *HitResult.GetActor()->GetName());
			if (HitResult.GetActor()->GetName() == "StaticMeshActor_1")		//바닥에 닿았다면
			{
				skill2Area->SetWorldLocation(HitResult.Location);			//스킬범위데칼을 뿌려준다.
			}
		}
	}

	if (isSkill3Attacking)
	{
		curSkill3Charge += DeltaTime * 1.25f;

		UUI_ActionPlayer2* OwnerWidget = Widget;
		if (OwnerWidget == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't find Widget"));
			return;
		}
		OwnerWidget->UpdateChargeBar();
	}
	else
	{
		curSkill3Charge = 0;
	}

	if (isUltimateAttacking)	//궁극기 사용시 카메라를 (-500, 0, 500)만큼 멀리한다
	{
		tpsCamComp->SetRelativeLocation(FMath::VInterpTo(tpsCamComp->GetRelativeLocation(),
			FVector(-500, 0, 500), GetWorld()->GetDeltaSeconds(), 3.0f));
		tpsCamComp->SetRelativeRotation(FMath::RInterpTo(tpsCamComp->GetRelativeRotation(),
			FRotator(-20, 0, 0), GetWorld()->GetDeltaSeconds(), 3.0f));
	}
	else
	{
		tpsCamComp->SetRelativeLocation(FMath::VInterpTo(tpsCamComp->GetRelativeLocation(),
			FVector(0, 0, 0), GetWorld()->GetDeltaSeconds(), 3.0f));
		tpsCamComp->SetRelativeRotation(FMath::RInterpTo(tpsCamComp->GetRelativeRotation(),
			FRotator(0, 0, 0), GetWorld()->GetDeltaSeconds(), 3.0f));
	}
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
	//공격 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("MouseRight"), IE_Pressed, this, &AActionPlayer2::RMB_Click);
	//스킬1 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Skill1"), IE_Pressed, this, &AActionPlayer2::Skill1Attack);
	//스킬2 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Skill2"), IE_Pressed, this, &AActionPlayer2::Skill2Area);
	//스킬3 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Skill3"), IE_Pressed, this, &AActionPlayer2::Skill3Attack);
	PlayerInputComponent->BindAction(TEXT("Skill3"), IE_Released, this, &AActionPlayer2::Skill3End);
	//스킬4 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Skill4"), IE_Pressed, this, &AActionPlayer2::Skill4Attack);
	//궁극기 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Ultimate"), IE_Pressed, this, &AActionPlayer2::UltimateAttack);
}

void AActionPlayer2::LMB_Click()
{
	//UE_LOG(LogTemp, Warning, TEXT("LMB_Click!"));
	if (!isAttacking && !isImpacting)			//공격중x 피격중x
	{
		auto movement = GetCharacterMovement();		//달리는 중이면 대쉬공격 실행
		if (movement->MaxWalkSpeed == runSpeed)
			DashAttack();
		else										//아니면 일반 공격
			NormalAttack();
	}
}

void AActionPlayer2::RMB_Click()
{
	if (turnskill2Area)
	{
		turnskill2Area = false;
		skill2Area->SetVisibility(false);
	}
}

void AActionPlayer2::NormalAttack()
{
	//구르기, 공격중, 피격중 공격X
	if (isRollingAnim || isAttacking || isSkill3Attacking || isUltimateAttacking || isImpacting) return;

	if (turnskill2Area)
	{
		Skill2Attack();
		return;
	}
	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	anim->PlayNormalAttackAnim();		//일반공격 애니메이션 on
	isAttacking = true;
}

void AActionPlayer2::DashAttack()
{
	//구르기, 공격중, 피격중 공격X
	if (isRollingAnim || isAttacking || isSkill3Attacking || isUltimateAttacking || isImpacting) return;

	if (turnskill2Area)
	{
		turnskill2Area = false;
		skill2Area->SetVisibility(false);
	}

	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	anim->PlayDashAttackAnim();		//대쉬공격 애니메이션 on
	anim->Montage_JumpToSection("Section_Start", anim->DashAttackMontage);
	isAttacking = true;
}

void AActionPlayer2::Skill1Attack()
{
	//구르기, 공격중, 쿨타임, 피격중이면 공격X
	if (isRollingAnim || isAttacking || isSkill3Attacking || isUltimateAttacking || isCoolTimeSkill1 || isImpacting) return;

	if (turnskill2Area)
	{
		turnskill2Area = false;
		skill2Area->SetVisibility(false);
	}

	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	anim->PlaySkill1AttackAnim();		//스킬1공격 애니메이션 on
	isAttacking = true;
	skill1CoolTime = maxSkill1CoolTime;
	GetWorld()->GetTimerManager().SetTimer(Skill1CoolTimerHandle, this, &AActionPlayer2::CoolDownSkill1, 1.0f, true);
	isCoolTimeSkill1 = true;		//스킬1 쿨타임 on
}

void AActionPlayer2::CoolDownSkill1()
{
	--skill1CoolTime;
	UUI_ActionPlayer2* OwnerWidget = this->Widget;
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

void AActionPlayer2::Skill2Area()
{
	//구르기, 공격중, 쿨타임, 피격중이면 공격X
	if (isRollingAnim || isAttacking || isSkill3Attacking || isUltimateAttacking || isCoolTimeSkill2 || isImpacting) return;

	if (turnskill2Area)
	{
		Skill2Attack();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill2Area"));
		turnskill2Area = true;
		skill2Area->SetVisibility(true);
	}
}

void AActionPlayer2::Skill2Attack()
{
	FVector skill2Location = HitResult.Location;		//라인트레이스 도착지점 받아두기
	skill2Location.Z += 1000.0f;
	turnskill2Area = false;
	skill2Area->SetVisibility(false);
	GetWorld()->SpawnActor<APlayer2_Skill2_Portal>(skill2AttackFactory, skill2Location, FRotator(0, 0, 0));

	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	anim->PlaySkill2AttackAnim();		//스킬2공격 애니메이션 on
	isAttacking = true;

	skill2CoolTime = maxSkill2CoolTime;
	GetWorld()->GetTimerManager().SetTimer(Skill2CoolTimerHandle, this, &AActionPlayer2::CoolDownSkill2, 1.0f, true);
	isCoolTimeSkill2 = true;		//스킬2 쿨타임 on
}

void AActionPlayer2::CoolDownSkill2()
{
	--skill2CoolTime;
	UUI_ActionPlayer2* OwnerWidget = this->Widget;
	if (OwnerWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Widget"));
		return;
	}
	OwnerWidget->UpdateSkill2CoolTime();

	if (skill2CoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Skill2CoolTimerHandle);
		isCoolTimeSkill2 = false;					//스킬2 사용가능
	}
}

void AActionPlayer2::Skill3Attack()
{
	//구르기, 공격중, 쿨타임, 피격중이면 공격X
	if (isRollingAnim || isAttacking || isUltimateAttacking || isCoolTimeSkill3 || isImpacting) return;

	if (turnskill2Area)
	{
		turnskill2Area = false;
		skill2Area->SetVisibility(false);
	}

	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	anim->PlaySkill3AttackAnim();		//스킬3공격 애니메이션 on
	isSkill3Attacking = true;

	UUI_ActionPlayer2* OwnerWidget = this->Widget;
	OwnerWidget->OnVisibilityChargeBar();			//차지바 보이도록 설정

	GetWorld()->GetTimerManager().SetTimer(Skill3EndHandle, this, &AActionPlayer2::Skill3End, 3.0f, true);
	skill3CoolTime = maxSkill3CoolTime;
	GetWorld()->GetTimerManager().SetTimer(Skill3CoolTimerHandle, this, &AActionPlayer2::CoolDownSkill3, 1.0f, true);
	isCoolTimeSkill3 = true;		//스킬3 쿨타임 on
}

void AActionPlayer2::CoolDownSkill3()
{
	--skill3CoolTime;
	UUI_ActionPlayer2* OwnerWidget = this->Widget;
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

void AActionPlayer2::Skill3End()
{
	GetWorld()->GetTimerManager().ClearTimer(Skill3EndHandle);		//스킬3 애니메이션 종료

	if (!isSkill3Attacking) return;

	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill3AttackMontage) return;

	anim->PlaySkill3AttackAnim();
	anim->Montage_JumpToSection("Charge_End", anim->Skill3AttackMontage);
	//isAttacking = false;						//노티파이에서 false시켜줌
}

void AActionPlayer2::Skill4Attack()
{
	//구르기, 공격중, 쿨타임, 피격중이면 공격X
	if (isRollingAnim || isAttacking || isSkill3Attacking || isUltimateAttacking || isCoolTimeSkill4 || isImpacting) return;

	if (turnskill2Area)
	{
		turnskill2Area = false;
		skill2Area->SetVisibility(false);
	}

	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->Skill4AttackMontage) return;

	anim->PlaySkill4AttackAnim();
	isAttacking = true;
	skill4CoolTime = maxSkill4CoolTime;
	GetWorld()->GetTimerManager().SetTimer(Skill4CoolTimerHandle, this, &AActionPlayer2::CoolDownSkill4, 1.0f, true);
	isCoolTimeSkill4 = true;		//스킬4 쿨타임 on
}

void AActionPlayer2::CoolDownSkill4()
{
	--skill4CoolTime;
	UUI_ActionPlayer2* OwnerWidget = this->Widget;
	if (OwnerWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Widget"));
		return;
	}
	OwnerWidget->UpdateSkill4CoolTime();

	if (skill4CoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Skill4CoolTimerHandle);
		isCoolTimeSkill4 = false;					//스킬4 사용가능
	}
}

void AActionPlayer2::UltimateAttack()
{
	//구르기, 공격중, 쿨타임, 피격중이면 공격X
	if (isRollingAnim || isAttacking || isSkill3Attacking || isCoolTimeUltimate|| isImpacting) return;

	if (turnskill2Area)
	{
		turnskill2Area = false;
		skill2Area->SetVisibility(false);
	}

	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->UltimateAttackMontage) return;

	anim->PlayUltimateAttackAnim();
	isUltimateAttacking = true;
	GetWorld()->GetTimerManager().SetTimer(UltimateHoldOffHandle, this, &AActionPlayer2::UltimateHoldOff, 4.5f, true);

	ultimateCoolTime = maxUltimateCoolTime;
	GetWorld()->GetTimerManager().SetTimer(UltimateCoolTimerHandle, this, &AActionPlayer2::CoolDownUltimate, 1.0f, true);
	isCoolTimeUltimate = true;		//궁극기 쿨타임 on
}

void AActionPlayer2::CoolDownUltimate()
{
	--ultimateCoolTime;
	UUI_ActionPlayer2* OwnerWidget = this->Widget;
	if (OwnerWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Widget"));
		return;
	}
	OwnerWidget->UpdateUltCoolTime();

	if (ultimateCoolTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Skill1CoolTimerHandle);
		isCoolTimeUltimate = false;					//궁극기 사용가능
	}
}

void AActionPlayer2::UltimateHoldOff()
{
	GetWorld()->GetTimerManager().ClearTimer(UltimateHoldOffHandle);
	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->UltimateAttackMontage) return;

	anim->PlayUltimateAttackAnim();
	anim->Montage_JumpToSection(FName("Hold off"), anim->UltimateAttackMontage);
}

void AActionPlayer2::PlayerDie()
{
	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	if (!anim || !anim->DieMontage) return;

	anim->PlayDieMontage();
	isImpacting = true;
	GetWorld()->GetTimerManager().SetTimer(DieDelayTimerHandle, this, &AActionPlayer2::GamePause, 2.0f, true);
}

void AActionPlayer2::GamePause()
{
	GetWorld()->GetTimerManager().ClearTimer(DieDelayTimerHandle);
	APlayerController* const MyPlayer = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (MyPlayer != NULL)
	{
		MyPlayer->SetPause(true);
	}
}

void AActionPlayer2::Turn(float value)
{
	if (isSkill3Attacking || isUltimateAttacking) return;
	AddControllerYawInput(value);
}

void AActionPlayer2::LookUp(float value)
{
	if (isSkill3Attacking || isUltimateAttacking) return;
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
	//구르기, 공격중, 쿨타임, 피격중이면 공격X
	if (isRollingAnim || isAttacking || isSkill3Attacking || isUltimateAttacking || isImpacting) return;

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
	//궁극기, 쿨타임, 피격중이면 공격X
	if (isUltimateAttacking || isCoolTimeRolling || isImpacting) return;

	if (turnskill2Area)
	{
		turnskill2Area = false;
		skill2Area->SetVisibility(false);
	}

	//구르기 애니메이션 재생
	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());

	anim->PlayDodgeRollAnim();		//구르기 애니메이션 on
	//공격중이었으면
	if (isAttacking)
		isAttacking = false;
	else if (isSkill3Attacking)
		isSkill3Attacking = false;

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
	if (isSkill3Attacking)
	{
		isSkill3Attacking = false;
		UUI_ActionPlayer2* OwnerWidget = this->Widget;
		OwnerWidget->OffVisibilityChargeBar();			//차지바 보이도록 설정
	}
		
	if (isUltimateAttacking)
		isUltimateAttacking = false;
	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	anim->StopAllMontages(0.2f);
}

void AActionPlayer2::CreateNormalAttackEffect()
{
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_NormalAttack>(normalAttackFactory, skillPosition);
}

void AActionPlayer2::CreateDashAttackEffect()
{
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_DashAttack>(dashAttackFactory, skillPosition);
}

void AActionPlayer2::CreateSkill1AttackEffect()
{
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_Skill1>(skill1AttackFactory, skillPosition);
}

void AActionPlayer2::CreateSkill3AttackEffect()
{
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_Skill3>(skill3AttackFactory, skillPosition);
}
void AActionPlayer2::CreateSkill4AttackEffect()
{
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_Skill4Factory>(skill4AttackFactory, skillPosition);
}

void AActionPlayer2::CreateUltimateAttackEffect()
{
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_UltimateFactory>(ultTornadoAttackFactory, skillPosition);
}

void AActionPlayer2::SmallImpactEnd()
{
	isImpacting = false;
}

float AActionPlayer2::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		playerHealth -= Damage;
		if (playerHealth < 0)
		{
			playerHealth = 0;
		}
		Widget->UpdateHealthBar();
	}

	if (playerHealth <= 0)
	{
		SetActorEnableCollision(false);
		PlayerDie();
		UE_LOG(LogTemp, Warning, TEXT("Player1 Die"));
		return ActualDamage;
	}

	//스킬공격, 궁극기, 대쉬공격, 구르기 중에는 피격모션 X
	if (isAttacking || isSkill3Attacking || isUltimateAttacking || isRollingAnim) return ActualDamage;

	auto anim = Cast<UPlayer2Anim>(GetMesh()->GetAnimInstance());
	anim->PlaySmallImpactMontage();

	isImpacting = true;
	if (isAttacking)					//일반공격중에 피격모션이 나왔을 경우
	{
		isAttacking = false;			//공격중을 false로
	}

	return ActualDamage;
}
