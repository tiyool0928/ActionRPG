// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBoss.generated.h"

UCLASS()
class ACTIONPROJECT_API AEnemyBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	AEnemyBoss* me = Cast<AEnemyBoss>(GetOwner());

	UPROPERTY(EditAnywhere, Category = EnemySetting)
		int maxHealth;
	UPROPERTY(EditAnywhere, Category = EnemySetting)
		int health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UArrowComponent* attackArrow;

	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class AEnemyBoss_Attack1> attack1Factory;				//스킬 1액터생성팩토리

	UPROPERTY(EditDefaultsOnly, Category = BossAnim)
		class UAnimMontage* Attack1Montage;

	void Attack();

	//노티파이 호출 함수
	UFUNCTION(BlueprintCallable)	//Attack1이펙트 타이밍
		void Attack1Effect();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
