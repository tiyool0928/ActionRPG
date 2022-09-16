// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActionPlayer1.generated.h"

UCLASS()
class ACTIONPROJECT_API AActionPlayer1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionPlayer1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* tpsCamComp;

	UPROPERTY(VisibleAnywhere, Category = WeaponMesh)
		class UStaticMeshComponent* weaponComp;				//무기메시 컴포넌트 등록
	UPROPERTY(EditAnywhere)
		class UBoxComponent* weaponBoxComp;					//무기 충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UArrowComponent* skillArrow;
	UPROPERTY(EditAnywhere, Category = Skill2)
		class UBoxComponent* skill2BoxComp;					//스킬2 충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Skill2)
		class UParticleSystemComponent* skill2EffectComp;	//이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = UltimateMesh)
		class UParticleSystemComponent* ultimateEffectComp;		//궁극기 파티클 컴포넌트 등록
	UPROPERTY(EditAnywhere)										
		class UBoxComponent* ultimateBoxComp;					//궁극기 충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Component)			
		class UPlayerBaseComponent* playerMove;					//이동 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Component)
		class UPlayerBaseComponent* playerAttack;				//공격 컴포넌트
	
	int player1MaxHealth;			//최대체력
	int player1Health;				//현재체력
	bool isAttacking;				//공격중인가?		(공격중엔 움직임 x, 구르기 o)
	bool isSkillAttacking;			//스킬공격중인가?		(일반공격중에 가능, 움직임x, 구르기 o, 대쉬어택 x)
	bool isSkill2Attacking;			//스킬2공격중인가?	(일반공격중에 가능, 움직임o, 구르기 o, 대쉬어택 x)
	bool isSkill4Flying;			//스킬4 공격모션중인가?	(구르기 x)
	bool isSkill4Releasing;			//스킬4 버튼을 누르고있는가?
	bool isUltimateAttacking;		//궁극기 모션중인가? (구르기 x)
	bool isAttackButtonWhenAttack;	//공격중에 공격을 눌렀는가
	bool isDashAttacking;			//대쉬공격중인가?		(활성화 될 경우 isAttacking과 함께 활성화)
	bool skill2Delay;				//스킬2 데미지 틱 딜레이를 위한 변수
	bool skill4FeverTime;			//스킬4 강화공격 타임이 켜졌는가?
	bool isCoolTimeSkill1;			//스킬1 쿨타임중인가?
	float skill1CoolTime;			//스킬1 쿨타임
	bool isCoolTimeSkill2;			//스킬2 쿨타임중인가?
	float skill2CoolTime;			//스킬2 쿨타임
	bool isCoolTimeSkill3;			//스킬3 쿨타임중인가?
	float skill3CoolTime;			//스킬3 쿨타임
	bool isCoolTimeSkill4;			//스킬4 쿨타임중인가?
	float skill4CoolTime;			//스킬4 쿨타임
	bool isCoolTimeUltimate;		//궁극기 쿨타임중인가?
	float ultimateCoolTime;			//궁극기 쿨타임
	TArray<AActor*> OverLapSkill2Actors;	//스킬2와 겹치는 액터배열
	void LMB_Click();				//일반 공격(마우스 왼쪽)을 눌렀을 때 함수

	void Skill2DamageDelay();		//스킬2 틱 데미지 딜레이 함수

	FTimerHandle Skill2DamageDelayHandle;		//스킬2 틱 데미지 딜레이 관리

	//노티파이 호출 함수
	UFUNCTION(BlueprintCallable)	//공격중에 한번 더 공격을 눌렀는지 확인 함수
		void AttackInputChecking();
	UFUNCTION(BlueprintCallable)	//공격이 끝났는지 확인 함수
		void EndAttacking();
	UFUNCTION(BlueprintCallable)	//공격의 데미지 영역 부분인지 확인 함수
		void AttackDamageApplying();
	UFUNCTION(BlueprintCallable)	//공격의 데미지 영역 부분이 끝나는 함수
		void AttackDamageEnd();
	UFUNCTION(BlueprintCallable)	//스킬1 이펙트 출력 함수
		void CreateSkill1Effect();
	UFUNCTION(BlueprintCallable)	//스킬3 이펙트 출력 함수
		void CreateSkill3Effect();
	UFUNCTION(BlueprintCallable)	//스킬4 모션 이후 구르기 가능 확인 함수
		void Skill4CanDodge();
	UFUNCTION(BlueprintCallable)	//구르기 잔상 생성 함수
		void CreateGhostTrail_Dodge();
	UFUNCTION(BlueprintCallable)	//스킬 잔상 생성 함수
		void CreateGhostTrail_Skill();

	//콜리전 함수
	UFUNCTION()
		void WeaponOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void Skill2OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void Skill2OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void UltimateSmashOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
