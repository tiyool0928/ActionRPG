// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2_Skill2.h"
#include "ActionPlayer2.h"
#include "Player2_Skill2_Boom.h"
#include "Components/BoxComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
APlayer2_Skill2::APlayer2_Skill2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	boxComp->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
	//외관 이펙트 컴포넌트
	effectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("effectComp"));
	effectComp->SetupAttachment(boxComp);
	effectComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempEffect(TEXT("ParticleSystem'/Game/Particles/P_P2_Skill2Effect.P_P2_Skill2Effect'"));
	if (TempEffect.Succeeded())
	{
		effectComp->SetTemplate(TempEffect.Object);
		//effectComp->SetRelativeLocation(FVector(0, 0, -250));
	}
	//발사체 컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(boxComp);
	movementComp->InitialSpeed = 700;
	movementComp->MaxSpeed = 700;
	movementComp->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void APlayer2_Skill2::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayer2_Skill2::OnOverlapBegin);
}

// Called every frame
void APlayer2_Skill2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayer2_Skill2::Die()
{
	Destroy();
}

void APlayer2_Skill2::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("OverlapActor : %s"), *OtherActor->GetName());
		if (OtherActor->GetName() == "StaticMeshActor_1" && !overlapCheck)
		{
			overlapCheck = true;
			FTransform skillPosition = boxComp->GetComponentTransform();
			GetWorld()->SpawnActor<APlayer2_Skill2_Boom>(skill2BoomFactory, skillPosition);

			if (camShake != NULL)
			{
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(camShake, 1.0f);
			}

			Die();
		}
	}
}