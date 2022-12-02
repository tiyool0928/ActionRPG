// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2_UltimateFactory.h"
#include "ActionPlayer2.h"
#include "Components/BoxComponent.h"
#include "Player2_Ultimate_Tornado1.h"
#include "Player2_Ultimate_Tornado2.h"
#include "Player2_Ultimate_Tornado3.h"
#include "Player2_Ultimate_Tornado4.h"
#include "Player2_Ultimate_Explosion.h"

// Sets default values
APlayer2_UltimateFactory::APlayer2_UltimateFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//루트 컴포넌트
	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("rootComp"));
	SetRootComponent(rootComp);

}

// Called when the game starts or when spawned
void APlayer2_UltimateFactory::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer2_UltimateFactory::Die, 4.0f, false);
	GetWorld()->GetTimerManager().SetTimer(boomTimerHandle, this, &APlayer2_UltimateFactory::CreateExplosion, 3.0f, false);
	CreateTornado();
}

// Called every frame
void APlayer2_UltimateFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayer2_UltimateFactory::Die()
{
	Destroy();
}

void APlayer2_UltimateFactory::CreateTornado()
{
	FTransform skillPosition = rootComp->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_Ultimate_Tornado1>(ultTornado1AttackFactory, skillPosition);
	GetWorld()->SpawnActor<APlayer2_Ultimate_Tornado2>(ultTornado2AttackFactory, skillPosition);
	GetWorld()->SpawnActor<APlayer2_Ultimate_Tornado3>(ultTornado3AttackFactory, skillPosition);
	GetWorld()->SpawnActor<APlayer2_Ultimate_Tornado4>(ultTornado4AttackFactory, skillPosition);
}

void APlayer2_UltimateFactory::CreateExplosion()
{
	FTransform skillPosition = rootComp->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_Ultimate_Explosion>(ultExplosionAttackFactory, skillPosition);
}
