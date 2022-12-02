// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2_Skill4.h"
#include "ActionPlayer2.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

// Sets default values
APlayer2_Skill4::APlayer2_Skill4()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//루트 컴포넌트
	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("rootBox"));
	SetRootComponent(rootComp);
	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxComp->SetupAttachment(rootComp);
	boxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	boxComp->SetRelativeLocation(FVector(100, 0, -30));
	boxComp->SetBoxExtent(FVector(70, 250, 70));
	//외관 이펙트 컴포넌트
	effectComp1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp1"));
	effectComp1->SetupAttachment(boxComp);
	effectComp1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	effectComp2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp2"));
	effectComp2->SetupAttachment(boxComp);
	effectComp2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	effectComp3 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp3"));
	effectComp3->SetupAttachment(boxComp);
	effectComp3->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/Particles/P_P2_Skill4Effect.P_P2_Skill4Effect'"));
	if (TempEffect.Succeeded())
	{
		effectComp1->SetTemplate(TempEffect.Object);
		effectComp1->SetRelativeLocation(FVector(0, -150, -50));
		effectComp1->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
		effectComp2->SetTemplate(TempEffect.Object);
		effectComp2->SetRelativeLocation(FVector(0, 0, -50));
		effectComp2->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
		effectComp3->SetTemplate(TempEffect.Object);
		effectComp3->SetRelativeLocation(FVector(0, 150, -50));
		effectComp3->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	}
}

// Called when the game starts or when spawned
void APlayer2_Skill4::BeginPlay()
{
	Super::BeginPlay();
	
	//충돌체에 overlapbegin 할당
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayer2_Skill4::OnOverlapBegin);

	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer2_Skill4::Die, 1.5f, false);
	GetWorld()->GetTimerManager().SetTimer(collisionTimerHandle, this, &APlayer2_Skill4::CollisionRemove, 0.5f, false);
}

// Called every frame
void APlayer2_Skill4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayer2_Skill4::Die()
{
	Destroy();
}

void APlayer2_Skill4::CollisionRemove()
{
	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayer2_Skill4::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto otherActor = Cast<AActionPlayer2>(OtherActor);

		if (OtherActor != otherActor)
			UGameplayStatics::ApplyDamage(OtherActor, 500.0f, nullptr, this, nullptr);
	}
}

