// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2_DashAttack.h"
#include "ActionPlayer2.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

// Sets default values
APlayer2_DashAttack::APlayer2_DashAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);
	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxComp->SetupAttachment(rootComp);
	boxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//boxComp->SetWorldScale3D(FVector(0.7f, 0.7f, 0.4f));
	//boxComp->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
	//외관 이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	effectComp->SetupAttachment(boxComp);
	effectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/Particles/P_P2_DashAttackEffect.P_P2_DashAttackEffect'"));
	if (TempEffect.Succeeded())
	{
		effectComp->SetTemplate(TempEffect.Object);
		//effectComp->SetRelativeLocation(FVector(0, 0, -250));
	}
}

// Called when the game starts or when spawned
void APlayer2_DashAttack::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayer2_DashAttack::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer2_DashAttack::Die, 0.5f, false);
}

// Called every frame
void APlayer2_DashAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayer2_DashAttack::Die()
{
	Destroy();
}

void APlayer2_DashAttack::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto me = Cast<AActionPlayer2>(OtherActor);

		if (OtherActor != me)
			UGameplayStatics::ApplyDamage(OtherActor, 100.0f, nullptr, this, nullptr);
	}
}

