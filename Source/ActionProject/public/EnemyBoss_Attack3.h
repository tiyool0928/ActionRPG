// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBoss_Attack3.generated.h"

UCLASS()
class ACTIONPROJECT_API AEnemyBoss_Attack3 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBoss_Attack3();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* rootComp;				//루트 컴포넌트
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp;				//충돌체 컴포넌트
	UPROPERTY(EditAnywhere, Category = EffectMesh)
		class UParticleSystemComponent* effectComp;			//이펙트 컴포넌트

	FTimerHandle deathTimerHandle;					//스킬이 끝나는 시간

	void Die();										//발사체 소멸

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
