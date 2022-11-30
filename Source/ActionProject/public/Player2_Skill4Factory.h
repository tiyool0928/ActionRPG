// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player2_Skill4Factory.generated.h"

UCLASS()
class ACTIONPROJECT_API APlayer2_Skill4Factory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayer2_Skill4Factory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* rootComp;			//루트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UArrowComponent* skillArrow;

	FTimerHandle deathTimerHandle;					//스킬이 끝나는 시간
	FTimerHandle Effect2TimerHandle;				//두번째 이펙트 타이머
	FTimerHandle Effect3TimerHandle;				//세번째 이펙트 타이머

	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_Skill4> skill4AttackFactory;				//스킬4액터생성팩토리

	void Die();

	void CreateEffect1();
	void CreateEffect2();
	void CreateEffect3();
};
