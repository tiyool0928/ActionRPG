// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2_Ultimate_Explosion.h"
#include "ActionPlayer2.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

// Sets default values
APlayer2_Ultimate_Explosion::APlayer2_Ultimate_Explosion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	boxComp->SetBoxExtent(FVector(128, 128, 128));
	boxComp->SetWorldScale3D(FVector(4, 4, 4));
	//외관 이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	effectComp->SetupAttachment(boxComp);
	effectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/Particles/P_P2_Ult_Explosion.P_P2_Ult_Explosion'"));
	if (TempEffect.Succeeded())
	{
		effectComp->SetTemplate(TempEffect.Object);
	}
}

// Called when the game starts or when spawned
void APlayer2_Ultimate_Explosion::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayer2_Ultimate_Explosion::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer2_Ultimate_Explosion::Die, 1.0f, false);
}

// Called every frame
void APlayer2_Ultimate_Explosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayer2_Ultimate_Explosion::Die()
{
	Destroy();
}

void APlayer2_Ultimate_Explosion::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto otherActor = Cast<AActionPlayer2>(OtherActor);

		if (OtherActor != otherActor)
			UGameplayStatics::ApplyDamage(OtherActor, 1500.0f, nullptr, this, nullptr);
	}
}