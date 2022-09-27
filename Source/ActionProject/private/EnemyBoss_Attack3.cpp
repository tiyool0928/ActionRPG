// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss_Attack3.h"
#include "EnemyBoss.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

// Sets default values
AEnemyBoss_Attack3::AEnemyBoss_Attack3()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//루트 컴포넌트
	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(rootComp);
	//충돌체 컴포넌트
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collider"));
	meshComp->SetupAttachment(rootComp);
	meshComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));
	if (TempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(TempMesh.Object);
		meshComp->SetRelativeLocation(FVector(0, 0, -100.0f));
		meshComp->SetWorldScale3D(FVector(6.0f, 6.0f, 3.0f));
	}
	//외관 이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	effectComp->SetupAttachment(rootComp);
	effectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/Particles/P_Mace_Impact_Damage.P_Mace_Impact_Damage'"));
	if (TempEffect.Succeeded())
	{
		effectComp->SetTemplate(TempEffect.Object);
		effectComp->SetRelativeLocation(FVector(0, 0, -100.0f));
		effectComp->SetWorldScale3D(FVector(2.0f, 2.0f, 1.0f));
	}
}

// Called when the game starts or when spawned
void AEnemyBoss_Attack3::BeginPlay()
{
	Super::BeginPlay();
	
	meshComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBoss_Attack3::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &AEnemyBoss_Attack3::Die, 0.5f, false);
}

// Called every frame
void AEnemyBoss_Attack3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBoss_Attack3::Die()
{
	Destroy();
}

void AEnemyBoss_Attack3::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto otherActor = Cast<AEnemyBoss>(OtherActor);

		if (OtherActor != otherActor)
			UGameplayStatics::ApplyDamage(OtherActor, 200.0f, nullptr, this, nullptr);
	}
}

