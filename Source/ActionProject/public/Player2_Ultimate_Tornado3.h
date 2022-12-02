// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player2_Ultimate_Tornado3.generated.h"

UCLASS()
class ACTIONPROJECT_API APlayer2_Ultimate_Tornado3 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayer2_Ultimate_Tornado3();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle deathTimerHandle;				//스킬이 끝나는 시간

	UPROPERTY(EditAnywhere)
		class UBoxComponent* rootComp;			//루트 컴포넌트
	UPROPERTY(EditAnywhere)
		class UBoxComponent* box1Comp;							//충돌체1 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = EffectMesh)
		class UParticleSystemComponent* effect1Comp;			//이펙트1 컴포넌트
	UPROPERTY(EditAnywhere)
		class UBoxComponent* box2Comp;							//충돌체2 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = EffectMesh)
		class UParticleSystemComponent* effect2Comp;			//이펙트2 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Movement)
		class URotatingMovementComponent* movementComp;			//회전 컴포넌트

	void Die();									//소멸

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
