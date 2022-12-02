// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2_Ultimate_Tornado2.h"
#include "ActionPlayer2.h"
#include "Components/BoxComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include <GameFramework/RotatingMovementComponent.h>

// Sets default values
APlayer2_Ultimate_Tornado2::APlayer2_Ultimate_Tornado2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//루트 컴포넌트
	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("rootComp"));
	SetRootComponent(rootComp);
	//충돌체 컴포넌트
	box1Comp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box1 Collider"));
	box1Comp->SetupAttachment(rootComp);
	box1Comp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	box1Comp->SetBoxExtent(FVector(70, 70, 200));
	box1Comp->SetRelativeLocation(FVector(0, 600, 0));
	box1Comp->SetWorldScale3D(FVector(0.7f, 0.7f, 1));

	box2Comp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box2 Collider"));
	box2Comp->SetupAttachment(rootComp);
	box2Comp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	box2Comp->SetBoxExtent(FVector(70, 70, 200));
	box2Comp->SetRelativeLocation(FVector(0, -600, 0));
	box2Comp->SetWorldScale3D(FVector(0.7f, 0.7f, 1));

	//외관 이펙트 컴포넌트
	effect1Comp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effect1Comp"));
	effect1Comp->SetupAttachment(box1Comp);
	effect1Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	effect2Comp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effect2Comp"));
	effect2Comp->SetupAttachment(box2Comp);
	effect2Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/Particles/P_P2_Ult_Tornado2Effect.P_P2_Ult_Tornado2Effect'"));
	if (TempEffect.Succeeded())
	{
		effect1Comp->SetTemplate(TempEffect.Object);
		effect1Comp->SetRelativeLocation(FVector(0, 0, -200));
		effect1Comp->SetWorldScale3D(FVector(0.7f, 0.7f, 0.7f));

		effect2Comp->SetTemplate(TempEffect.Object);
		effect2Comp->SetRelativeLocation(FVector(0, 0, -200));
		effect2Comp->SetWorldScale3D(FVector(0.7f, 0.7f, 0.7f));
	}
	//회전 컴포넌트
	movementComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(rootComp);
}

// Called when the game starts or when spawned
void APlayer2_Ultimate_Tornado2::BeginPlay()
{
	Super::BeginPlay();
	box1Comp->OnComponentBeginOverlap.AddDynamic(this, &APlayer2_Ultimate_Tornado2::OnOverlapBegin);
	box2Comp->OnComponentBeginOverlap.AddDynamic(this, &APlayer2_Ultimate_Tornado2::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer2_Ultimate_Tornado2::Die, 3.0f, false);
}

// Called every frame
void APlayer2_Ultimate_Tornado2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayer2_Ultimate_Tornado2::Die()
{
	Destroy();
}

void APlayer2_Ultimate_Tornado2::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto me = Cast<AActionPlayer2>(OtherActor);

		if (OtherActor != me)
		{
			UGameplayStatics::ApplyDamage(OtherActor, 500.0f, nullptr, this, nullptr);
		}
	}
}

