// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2_NormalAttack.h"
#include "ActionPlayer2.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include <GameFramework/ProjectileMovementComponent.h>

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
	boxComp->SetRelativeLocation(FVector(175, 0, 0));
	boxComp->SetWorldScale3D(FVector(1.5f, 0.7f, 0.2f));
	boxComp->SetBoxExtent(FVector(50.0f, 100.0f, 300.0f));
	//외관 이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	effectComp->SetupAttachment(boxComp);
	effectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/Particles/P_Split_Small.P_Split_Small'"));
	if (TempEffect.Succeeded())
	{
		effectComp->SetTemplate(TempEffect.Object);
		effectComp->SetRelativeLocation(FVector(-55, 0, 0));
		effectComp->SetRelativeRotation(FRotator(-90, 0, 0));
		effectComp->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	}

	//발사체 컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(boxComp);
	movementComp->InitialSpeed = 500;
	movementComp->MaxSpeed = 500;
	movementComp->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void APlayer2_NormalAttack::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayer2_NormalAttack::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer2_NormalAttack::Die, 1.0f, false);
	
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

void APlayer2_NormalAttack::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto me = Cast<AActionPlayer2>(OtherActor);

		if (OtherActor != me)
			UGameplayStatics::ApplyDamage(OtherActor, 50.0f, nullptr, this, nullptr);
	}
}

