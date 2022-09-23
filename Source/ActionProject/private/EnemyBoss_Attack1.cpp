// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss_Attack1.h"
#include "EnemyBoss.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

// Sets default values
AEnemyBoss_Attack1::AEnemyBoss_Attack1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//루트 컴포넌트
	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(rootComp);
	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxComp->SetupAttachment(rootComp);
	boxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	boxComp->SetRelativeLocationAndRotation(FVector(80.0f, -10.0f, 10.0f), FRotator(0, 0, -10.0f));
	boxComp->SetWorldScale3D(FVector(1.75f, 1.0f, 4.0f));
	//외관 이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	effectComp->SetupAttachment(rootComp);
	effectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/Particles/P_Genno_BellyBump_Ground_01.P_Genno_BellyBump_Ground_01'"));
	if (TempEffect.Succeeded())
	{
		effectComp->SetTemplate(TempEffect.Object);
		effectComp->SetRelativeRotation(FRotator(180.0f, 0, 100.0f));
		effectComp->SetWorldScale3D(FVector(1.0f, 1.5f, 1.0f));
	}
}

// Called when the game starts or when spawned
void AEnemyBoss_Attack1::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBoss_Attack1::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &AEnemyBoss_Attack1::Die, 0.1f, false);
}

// Called every frame
void AEnemyBoss_Attack1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBoss_Attack1::Die()
{
	Destroy();
}

void AEnemyBoss_Attack1::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto otherActor = Cast<AEnemyBoss>(OtherActor);

		if (OtherActor != otherActor)
			UGameplayStatics::ApplyDamage(OtherActor, 10.0f, nullptr, this, nullptr);
	}
}

