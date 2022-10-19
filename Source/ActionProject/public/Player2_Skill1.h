// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player2_Skill1.generated.h"

UCLASS()
class ACTIONPROJECT_API APlayer2_Skill1 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayer2_Skill1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle deathTimerHandle;				//스킬이 끝나는 시간

	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxComp;			//충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = EffectMesh)
		class UParticleSystemComponent* effectComp;			//이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Movement)
		class UProjectileMovementComponent* movementComp;			//발사체 이동 컴포넌트

	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_Skill1_Boom> skill1BoomFactory;			//스킬1폭파이펙트생성팩토리

	bool overlapCheck = false;

	void Die();									//발사체 소멸

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
