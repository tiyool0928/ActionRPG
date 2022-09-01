// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLog.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEnemyLog::AEnemyLog()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));						//충돌체 컴포넌트
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(70.0f, 70.0f, 200.0f));

	boxComp->SetWorldScale3D(FVector(1, 1, 1));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));		//메시 컴포넌트
	meshComp->SetupAttachment(boxComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempLogMesh(TEXT("StaticMesh'/Game/Mesh/Log/uploads_files_2548522_fbx_blendtree.uploads_files_2548522_fbx_blendtree'"));
	if (TempLogMesh.Succeeded())
	{
		meshComp->SetStaticMesh(TempLogMesh.Object);
		meshComp->SetRelativeRotation(FRotator(90, 0, 0));
	}

}

// Called when the game starts or when spawned
void AEnemyLog::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyLog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

