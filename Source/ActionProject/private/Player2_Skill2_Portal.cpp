// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2_Skill2_Portal.h"
#include "Player2_Skill2.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
APlayer2_Skill2_Portal::APlayer2_Skill2_Portal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//외관 이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	SetRootComponent(effectComp);
	effectComp->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/Particles/P_P2_Skill2Effect_Portal.P_P2_Skill2Effect_Portal'"));

	//메테오 방향 arrow 컴포넌트
	meteorDir = CreateDefaultSubobject<UArrowComponent>(TEXT("meteorArrow"));
	meteorDir->SetupAttachment(RootComponent);
	meteorDir->SetRelativeRotation(FRotator(-90.0f, 0, 0));
}

// Called when the game starts or when spawned
void APlayer2_Skill2_Portal::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer2_Skill2_Portal::Die, 2.0f, false);
	GetWorld()->GetTimerManager().SetTimer(meteorTimerHandle, this, &APlayer2_Skill2_Portal::CreateMeteor, 1.0f, false);
}

// Called every frame
void APlayer2_Skill2_Portal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayer2_Skill2_Portal::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("DIE"));
	Destroy();
}

void APlayer2_Skill2_Portal::CreateMeteor()
{
	FTransform skill2PortalPosition = meteorDir->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_Skill2>(meteorFactory, skill2PortalPosition);
}

