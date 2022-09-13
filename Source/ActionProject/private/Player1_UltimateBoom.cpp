// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1_UltimateBoom.h"
#include "ActionPlayer1.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APlayer1_UltimateBoom::APlayer1_UltimateBoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//루트 컴포넌트
	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Root Comp"));
	SetRootComponent(rootComp);
	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxComp->SetupAttachment(rootComp);
	boxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	boxComp->SetRelativeLocation(FVector(330, 0, 0));
	boxComp->SetBoxExtent(FVector(400.0f, 300.0f, 32.0f));
	//외관 이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	effectComp->SetupAttachment(rootComp);
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("ParticleSystem'/Game/Particles/Ultimate_BoomEffect.Ultimate_BoomEffect'"));

	if (tempEffect.Succeeded())
	{
		effectComp->SetTemplate(tempEffect.Object);
		effectComp->SetRelativeLocation(FVector(350, -8, -80));
		effectComp->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
	}
}

// Called when the game starts or when spawned
void APlayer1_UltimateBoom::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayer1_UltimateBoom::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer1_UltimateBoom::Die, 0.5f, false);
}

// Called every frame
void APlayer1_UltimateBoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayer1_UltimateBoom::Die()
{
	boxComp->DestroyComponent();
}

void APlayer1_UltimateBoom::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto otherActor = Cast<AActionPlayer1>(OtherActor);

		if (OtherActor != otherActor)
			UGameplayStatics::ApplyDamage(OtherActor, 2000.0f, nullptr, this, nullptr);
	}
}

