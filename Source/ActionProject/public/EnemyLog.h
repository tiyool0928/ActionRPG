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

	FTimerHandle Shake1TimerHandle;	//피격 흔들림 타이머
	FTimerHandle Shake2TimerHandle;	//피격 흔들림 타이머
	FTimerHandle HealTimerHandle;	//자동 회복 타이머(1초)
	bool healCoolTime;				//자동 회복 쿨타임 변수
	bool isBeingHit;				//피격 당했는가(피격모션을 위한 딜레이 변수)

	UPROPERTY(EditAnywhere, Category = EnemySetting)
		int maxHealth;
	UPROPERTY(EditAnywhere, Category = EnemySetting)
		int health;

	void ShakeMesh();				//메시 피격 시 흔들기
	void Shake1Delay();				//피격1 딜레이
	void Shake2Delay();				//피격2 딜레이
	void HealTimerDelay();			//자동 회복 딜레이
	void Heal();					//자동 회복 함수



	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
