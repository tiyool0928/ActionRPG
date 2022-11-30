// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player2_Skill4.generated.h"

UCLASS()
class ACTIONPROJECT_API APlayer2_Skill4 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayer2_Skill4();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle deathTimerHandle;				//스킬이 끝나는 시간
	FTimerHandle collisionTimerHandle;				//충돌체가 끝나는 시간

	UPROPERTY(EditAnywhere)
		class UBoxComponent* rootComp;			//루트 컴포넌트
	UPROPERTY(EditDefaultsOnly, Category = Collider)
		class UBoxComponent* boxComp;			//충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = EffectMesh)
		class UParticleSystemComponent* effectComp1;			//이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = EffectMesh)
		class UParticleSystemComponent* effectComp2;			//이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = EffectMesh)
		class UParticleSystemComponent* effectComp3;			//이펙트 컴포넌트

	void Die();									//발사체 소멸
	void CollisionRemove();						//충돌체 해제

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
