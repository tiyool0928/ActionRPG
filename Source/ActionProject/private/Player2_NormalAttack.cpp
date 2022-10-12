// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2_NormalAttack.h"
#include "ActionPlayer2.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

// Sets default values
APlayer2_NormalAttack::APlayer2_NormalAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//루트 컴포넌트
	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Root Comp"));
	SetRootComponent(rootComp);
	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxComp->SetupAttachment(rootComp);
	boxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	boxComp->SetWorldScale3D(FVector(2.0f, 0.7f, 0.2f));
	boxComp->SetBoxExtent(FVector(50.0f, 100.0f, 300.0f));
	//외관 이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	effectComp->SetupAttachment(rootComp);
	effectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/Particles/P_Split_Small.P_Split_Small'"));
	if (TempEffect.Succeeded())
	{
		effectComp->SetTemplate(TempEffect.Object);
		effectComp->SetRelativeLocation(FVector(-42, 0, -250));
		effectComp->SetRelativeRotation(FRotator(-90, 0, 0));
	}
}

// Called when the game starts or when spawned
void APlayer2_NormalAttack::BeginPlay()
{
	Super::BeginPlay();

	//boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayer2_NormalAttack::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer2_NormalAttack::Die, 1.5f, false);
	
}

// Called every frame
void APlayer2_NormalAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayer2_NormalAttack::Die()
{
	Destroy();
}

