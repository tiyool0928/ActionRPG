// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player2_Skill3.generated.h"

UCLASS()
class ACTIONPROJECT_API APlayer2_Skill3 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayer2_Skill3();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle deathTimerHandle;				//스킬이 끝나는 시간
	FTimerHandle damageDelayHandle;				//데미지 딜레이

	UPROPERTY(EditAnywhere)
		class UBoxComponent* rootComp;			//루트 컴포넌트
	UPROPERTY(EditDefaultsOnly, Category = Collider)
		class UBoxComponent* boxComp;			//충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = EffectMesh)
		class UParticleSystemComponent* effectComp;			//이펙트 컴포넌트

	void Die();									//발사체 소멸
	
	bool isDamageDelay = false;

	TArray<AActor*> OverLapSkill3Actors;	//스킬2와 겹치는 액터배열

	void DamageDelayOff();

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
