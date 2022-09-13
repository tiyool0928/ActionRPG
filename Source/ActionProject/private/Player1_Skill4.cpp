// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1_Skill4.h"
#include "ActionPlayer1.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APlayer1_Skill4::APlayer1_Skill4()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	boxComp->SetBoxExtent(FVector(400.0f, 400.0f, 100.0f));
	//외관 이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	effectComp->SetupAttachment(boxComp);
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("ParticleSystem'/Game/Particles/Skill4_FeverEffect.Skill4_FeverEffect'"));

	if (tempEffect.Succeeded())
	{
		effectComp->SetTemplate(tempEffect.Object);
		effectComp->SetRelativeLocation(FVector(20, 0, -70));
		effectComp->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
}

// Called when the game starts or when spawned
void APlayer1_Skill4::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayer1_Skill4::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer1_Skill4::Die, 0.5f, false);
}

// Called every frame
void APlayer1_Skill4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayer1_Skill4::Die()
{
	boxComp->DestroyComponent();
}

void APlayer1_Skill4::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto otherActor = Cast<AActionPlayer1>(OtherActor);

		if (OtherActor != otherActor)
			UGameplayStatics::ApplyDamage(OtherActor, 1000.0f, nullptr, this, nullptr);
	}
}

