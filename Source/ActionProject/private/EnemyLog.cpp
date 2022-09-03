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

	health = 10000.0f;						//통나무 체력
	isBeingHit = false;
}

// Called when the game starts or when spawned
void AEnemyLog::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("%f"), health);
}

// Called every frame
void AEnemyLog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//피격 함수
float AEnemyLog::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		health -= Damage;
		ShakeMesh();
		UE_LOG(LogTemp, Warning, TEXT("%f"), health);
	}

	return ActualDamage;
}

void AEnemyLog::ShakeMesh()
{
	if (isBeingHit) return;

	isBeingHit = true;
	meshComp->SetRelativeLocation(FVector(0, -10, 0));
	GetWorldTimerManager().SetTimer(Shake1TimerHandle, this, &AEnemyLog::Shake1Delay, 0.05f, true);
	GetWorldTimerManager().SetTimer(Shake2TimerHandle, this, &AEnemyLog::Shake2Delay, 0.1f, true);
	isBeingHit = false;
}

void AEnemyLog::Shake1Delay()
{
	GetWorldTimerManager().ClearTimer(Shake1TimerHandle);
	meshComp->SetRelativeLocation(FVector(0, 10, 0));
}

void AEnemyLog::Shake2Delay()
{
	GetWorldTimerManager().ClearTimer(Shake2TimerHandle);
	meshComp->SetRelativeLocation(FVector(0, 0, 0));
}