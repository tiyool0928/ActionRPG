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

	UPROPERTY(EditAnywhere, Category = "Components")
		class UBoxComponent* attack3SpinComp;

	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class AEnemyBoss_Attack1> attack1Factory;				//공격 1액터생성팩토리

	UPROPERTY(EditDefaultsOnly, Category = Factory)							//공격 2액터생성팩토리
		TSubclassOf<class AEnemyBoss_Attack1> attack2Factory;				//공격 1 액터를 재활용

	UPROPERTY(EditDefaultsOnly, Category = Factory)							
		TSubclassOf<class AEnemyBoss_Attack3> attack3Factory;				//공격 3액터생성팩토리

	UPROPERTY(EditDefaultsOnly, Category = BossAnim)
		class UAnimMontage* Attack1Montage;									//한번 휘두르기

	UPROPERTY(EditDefaultsOnly, Category = BossAnim)
		class UAnimMontage* Attack2Montage;									//3번 휘두르기

	UPROPERTY(EditDefaultsOnly, Category = BossAnim)
		class UAnimMontage* Attack3Montage;									//회전 후 주변 도넛 모양 광역딜

	void Attack1();
	void Attack2();
	void Attack3();

	//노티파이 호출 함수
	UFUNCTION(BlueprintCallable)	//Attack1이펙트 타이밍
		void Attack1Effect();
	UFUNCTION(BlueprintCallable)	//Attack2이펙트 타이밍
		void Attack2Effect();
	UFUNCTION(BlueprintCallable)	//Attack3 회전 모션 시작
		void Attack3SpinStart();
	UFUNCTION(BlueprintCallable)	//Attack3 회전 모션 끝
		void Attack3SpinEnd();
	UFUNCTION(BlueprintCallable)	//Attack3이펙트 타이밍
		void Attack3Effect();

	//콜리전 함수
	UFUNCTION()
		void Attack3SpinOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
