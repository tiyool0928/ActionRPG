// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2_Skill3.h"
#include "ActionPlayer2.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

// Sets default values
APlayer2_Skill3::APlayer2_Skill3()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//루트 컴포넌트
	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("rootBox"));
	SetRootComponent(rootComp);
	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxComp->SetupAttachment(rootComp);
	boxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	boxComp->SetRelativeLocation(FVector(350, 30, 30));
	//외관 이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	effectComp->SetupAttachment(boxComp);
	effectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/Particles/P_P2_Skill3Effect.P_P2_Skill3Effect'"));
	if (TempEffect.Succeeded())
	{
		effectComp->SetTemplate(TempEffect.Object);
		effectComp->SetRelativeLocationAndRotation(FVector(-300.0f, 0, 0), FRotator(-90.0f, 0, 0));
		effectComp->SetWorldScale3D(FVector(0.5f, 0.5f, 1));
	}
}

// Called when the game starts or when spawned
void APlayer2_Skill3::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer2_Skill3::Die, 2.0f, false);

	//충돌체에 overlapbegin 할당
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayer2_Skill3::OnOverlapBegin);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &APlayer2_Skill3::OnOverlapEnd);
}

// Called every frame
void APlayer2_Skill3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverLapSkill3Actors.Num() > 0)				//스킬3를 쓰는중에 액터가 들어와있으면
	{
		if (!isDamageDelay)
		{
			for (int i = 0; i < OverLapSkill3Actors.Num(); i++)
			{
				UGameplayStatics::ApplyDamage(OverLapSkill3Actors[i], 300.0f, nullptr, this, nullptr);
			}
			isDamageDelay = true;
			GetWorldTimerManager().SetTimer(damageDelayHandle, this, &APlayer2_Skill3::DamageDelayOff, 0.5f, true);
		}
	}
	AActionPlayer2* player = Cast<AActionPlayer2>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player->isSkill3Attacking)
	{
		Die();
	}
}

void APlayer2_Skill3::Die()
{
	Destroy();
}

void APlayer2_Skill3::DamageDelayOff()
{
	GetWorld()->GetTimerManager().ClearTimer(damageDelayHandle);		//스킬3 틱데미지 딜레이 초기화
	isDamageDelay = false;
}

void APlayer2_Skill3::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
		OverLapSkill3Actors.AddUnique(OtherActor);			//skill3를 맞고있는 액터배열에 추가
}

void APlayer2_Skill3::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverLapSkill3Actors.Remove(OtherActor);
}

