// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player2_DashAttack.generated.h"

UCLASS()
class ACTIONPROJECT_API APlayer2_DashAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayer2_DashAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* rootComp;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxComp;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* effectComp;

	FTimerHandle deathTimerHandle;					//이펙트가 끝나는 시간

	void Die();

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
