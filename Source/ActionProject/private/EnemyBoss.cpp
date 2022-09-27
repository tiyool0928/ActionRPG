// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"
#include "EnemyBossAnim.h"
#include "BossAIController.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "EnemyBoss_Attack1.h"
#include "EnemyBoss_Attack3.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
AEnemyBoss::AEnemyBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//컴포넌트 위치 (0, 0, -90) 회전 (0, -90, 0)
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	//공격arrow 컴포넌트
	attackArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("attackArrow"));
	attackArrow->SetupAttachment(RootComponent);
	attackArrow->SetRelativeLocation(FVector(40, 0, 20));

	//attack3 spin 충돌체 컴포넌트
	attack3SpinComp = CreateDefaultSubobject<UBoxComponent>(TEXT("attack3Spin"));
	attack3SpinComp->SetupAttachment(RootComponent);

	AIControllerClass = ABossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	maxHealth = 200000;						//보스 체력
	health = maxHealth;
}

// Called when the game starts or when spawned
void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BossHealth: %d"), health);

	//무기 충돌체에 overlapbegin 할당
	attack3SpinComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	attack3SpinComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBoss::Attack3SpinOnOverlapBegin);
}

// Called every frame
void AEnemyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBoss::Attack1()
{
	if (Attack1Montage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Didn't Played Attack1"));
		return;
	}

	PlayAnimMontage(Attack1Montage);
	
}

void AEnemyBoss::Attack2()
{
	if (Attack2Montage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Didn't Played Attack2"));
		return;
	}

	PlayAnimMontage(Attack2Montage);
}

void AEnemyBoss::Attack3()
{
	if (Attack3Montage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Didn't Played Attack3"));
		return;
	}

	PlayAnimMontage(Attack3Montage);
}

void AEnemyBoss::Attack1Effect()
{
	FTransform skillPosition = attackArrow->GetComponentTransform();
	GetWorld()->SpawnActor<AEnemyBoss_Attack1>(attack1Factory, skillPosition);
}

void AEnemyBoss::Attack2Effect()
{
	FTransform skillPosition = attackArrow->GetComponentTransform();
	GetWorld()->SpawnActor<AEnemyBoss_Attack1>(attack2Factory, skillPosition);
}

void AEnemyBoss::Attack3SpinStart()
{
	attack3SpinComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyBoss::Attack3SpinEnd()
{
	attack3SpinComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyBoss::Attack3Effect()
{
	FTransform skillPosition = attackArrow->GetComponentTransform();
	GetWorld()->SpawnActor<AEnemyBoss_Attack3>(attack3Factory, skillPosition);
}

void AEnemyBoss::Attack3SpinOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto otherActor = Cast<AEnemyBoss>(OtherActor);

		if (OtherActor != otherActor)
			UGameplayStatics::ApplyDamage(OtherActor, 50.0f, nullptr, this, nullptr);
	}
}

//피격 함수
float AEnemyBoss::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		health -= Damage;
		UE_LOG(LogTemp, Warning, TEXT("BossHealth: %d"), health);
	}

	return ActualDamage;
}
