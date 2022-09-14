// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostTrail.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
AGhostTrail::AGhostTrail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("Poseable Mesh"));
	PoseableMesh->SetupAttachment(RootComp);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PoseMesh(TEXT("SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (SK_PoseMesh.Succeeded())
	{
		PoseableMesh->SetSkeletalMesh(SK_PoseMesh.Object);
		PoseableMesh->SetRelativeLocation(FVector(0, 0, -90));
		PoseableMesh->SetRelativeRotation(FRotator(0, -90, 0));
	}
	ConstructorHelpers::FObjectFinder<UMaterialInstance> M_GhostTrail(TEXT("MaterialInstanceConstant'/Game/BluePrints/GhostTrail/MI_GhostTrail_Dodge.MI_GhostTrail_Dodge'"));
	if (M_GhostTrail.Succeeded())
	{
		GhostMaterial = M_GhostTrail.Object;
	}
}

// Called when the game starts or when spawned
void AGhostTrail::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSpawned)
	{
		FadeCountDown -= DeltaTime;
		for (int i = 0; i < Materials.Num(); i++)
		{
			Materials[i]->SetScalarParameterValue("Opacity", FadeCountDown / FadeOutTime);
		}
		if (FadeCountDown < 0)
		{
			Destroy();
		}
	}
}

void AGhostTrail::Init(USkeletalMeshComponent* Pawn)
{
	UE_LOG(LogTemp, Warning, TEXT("GhostBegin"));
	PoseableMesh->CopyPoseFromSkeletalComponent(Pawn);
	TArray<UMaterialInterface*> Mats = PoseableMesh->GetMaterials();

	for (int i = 0; i < Mats.Num(); i++)
	{
		Materials.Add(UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), GhostMaterial));
		PoseableMesh->SetMaterial(i, Materials[i]);
	}
	FadeCountDown = FadeOutTime;
	IsSpawned = true;
}

