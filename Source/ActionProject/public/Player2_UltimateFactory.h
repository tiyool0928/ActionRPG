// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player2_UltimateFactory.generated.h"

UCLASS()
class ACTIONPROJECT_API APlayer2_UltimateFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayer2_UltimateFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle deathTimerHandle;				//팩토리 소멸 타이머
	FTimerHandle boomTimerHandle;				//폭발 타이머

	UPROPERTY(EditAnywhere)
		class UBoxComponent* rootComp;			//루트 컴포넌트

	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_Ultimate_Tornado1> ultTornado1AttackFactory;	//궁극기 토네이도1 액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_Ultimate_Tornado2> ultTornado2AttackFactory;	//궁극기 토네이도2 액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_Ultimate_Tornado3> ultTornado3AttackFactory;	//궁극기 토네이도3 액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_Ultimate_Tornado4> ultTornado4AttackFactory;	//궁극기 토네이도4 액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_Ultimate_Explosion> ultExplosionAttackFactory;	//궁극기 토네이도4 액터생성팩토리

	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShakeBase> camShake;

	void Die();									//소멸
	void CreateTornado();						//토네이도 생성
	void CreateExplosion();						//폭발 생성
};
