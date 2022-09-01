// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyLog.generated.h"

UCLASS()
class ACTIONPROJECT_API AEnemyLog : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyLog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxComp;			//충돌체 컴포넌트
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp;	//메시 컴포넌트
};
