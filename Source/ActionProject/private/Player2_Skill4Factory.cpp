// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2_Skill4Factory.h"
#include "Player2_Skill4.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
APlayer2_Skill4Factory::APlayer2_Skill4Factory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//루트 컴포넌트
	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("rootBox"));
	SetRootComponent(rootComp);
	skillArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("skillArrow"));
	skillArrow->SetupAttachment(rootComp);
}

// Called when the game starts or when spawned
void APlayer2_Skill4Factory::BeginPlay()
{
	Super::BeginPlay();
	CreateEffect1();
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &APlayer2_Skill4Factory::Die, 1.5f, true);
	GetWorld()->GetTimerManager().SetTimer(Effect2TimerHandle, this, &APlayer2_Skill4Factory::CreateEffect2, 0.5f, true);
	GetWorld()->GetTimerManager().SetTimer(Effect3TimerHandle, this, &APlayer2_Skill4Factory::CreateEffect3, 1.0f, true);
}

// Called every frame
void APlayer2_Skill4Factory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayer2_Skill4Factory::Die()
{
	Destroy();
}

void APlayer2_Skill4Factory::CreateEffect1()
{
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_Skill4>(skill4AttackFactory, skillPosition);
}

void APlayer2_Skill4Factory::CreateEffect2()
{
	skillArrow->SetRelativeLocation(FVector(200, 0, 0));
	GetWorld()->GetTimerManager().ClearTimer(Effect2TimerHandle);
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_Skill4>(skill4AttackFactory, skillPosition);
}

void APlayer2_Skill4Factory::CreateEffect3()
{
	skillArrow->SetRelativeLocation(FVector(400, 0, 0));
	GetWorld()->GetTimerManager().ClearTimer(Effect3TimerHandle);
	FTransform skillPosition = skillArrow->GetComponentTransform();
	GetWorld()->SpawnActor<APlayer2_Skill4>(skill4AttackFactory, skillPosition);
}

