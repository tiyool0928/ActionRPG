// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1_Skill4Landing.h"
#include "ActionPlayer1.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APlayer1_Skill4Landing::APlayer1_Skill4Landing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	//이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	effectComp->SetupAttachment(boxComp);
	effectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/Particles/P_TapVortex_Activate.P_TapVortex_Activate'"));

	if (TempEffect.Succeeded())
	{
		effectComp->SetTemplate(TempEffect.Object);
	}
}

// Called when the game starts or when spawned
void APlayer1_Skill4Landing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayer1_Skill4Landing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

