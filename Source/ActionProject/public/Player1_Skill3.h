// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player1_Skill3.generated.h"

UCLASS()
class ACTIONPROJECT_API APlayer1_Skill3 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayer1_Skill3();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxComp;			//충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = EffectMesh)
		class UParticleSystemComponent* effectComp1;			//큰 이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = EffectMesh)
		class UParticleSystemComponent* effectComp2;			//중간 이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = EffectMesh)
		class UParticleSystemComponent* effectComp3;			//작은 이펙트 컴포넌트

	FTimerHandle deathTimerHandle;					//스킬이 끝나는 시간

	void Die();									//이펙트, 충돌체 소멸

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
