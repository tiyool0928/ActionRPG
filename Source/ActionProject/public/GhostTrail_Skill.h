// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostTrail_Skill.generated.h"

class UPoseableMeshComponent;
class UMaterialInstance;
class UMaterialInstanceDynamic;
class USkeletalMeshComponent;

UCLASS()
class ACTIONPROJECT_API AGhostTrail_Skill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhostTrail_Skill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(USkeletalMeshComponent* Pawn);

private:

	UPROPERTY()
		class UCapsuleComponent* RootComp;
	UPROPERTY(EditDefaultsOnly)
		UPoseableMeshComponent* PoseableMesh;			//잔상 mesh
	UMaterialInstance* GhostMaterial;				//잔상 material
	TArray<UMaterialInstanceDynamic*> Materials;	//UMaterialInstanceDynamic으로 받아야 파라미터도 저장됨

	bool IsSpawned = false;							//spawn이 되었는가?
	float FadeCountDown;
	float FadeOutTime = 0.5f;						//사라지는 속도

};
