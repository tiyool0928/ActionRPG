// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayer2.h"
#include "Player2Anim.h"
#include "GhostTrail.h"
#include <GameFramework/CharacterMovementComponent.h>

AActionPlayer2::AActionPlayer2()
{
	PrimaryActorTick.bCanEverTick = true;
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
