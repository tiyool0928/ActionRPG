// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1_Skill3.h"
#include "ActionPlayer1.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

// Sets default values
APlayer1_Skill3::APlayer1_Skill3()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	boxComp->SetWorldScale3D(FVector(3.0f, 3.0f, 2.0f));
	boxComp->SetBoxExtent(FVector(150.0f, 150.0f, 32.0f));
	//외관 이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	effectComp->SetupAttachment(boxComp);
	effectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_Whirlwind/P_Whirlwind_Default_WeaponTrail_01.P_Whirlwind_Default_WeaponTrail_01'"));
	if (TempEffect.Succeeded())
	{
		effectComp->SetTemplate(TempEffect.Object);
		effectComp->SetRelativeLocation(FVector(0, 0, -20));
	}
}

// Called when the game starts or when spawned
void APlayer1_Skill3::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayer1_Skill3::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer1_Skill3::Die, 0.5f, false);
}

// Called every frame
void APlayer1_Skill3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayer1_Skill3::Die()
{
	Destroy();
}

void APlayer1_Skill3::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto otherActor = Cast<AActionPlayer1>(OtherActor);

		if (OtherActor != otherActor)
			UGameplayStatics::ApplyDamage(OtherActor, 500.0f, nullptr, this, nullptr);
	}
}

