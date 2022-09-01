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

	//좌우 입력 처리
	void Turn(float value);
	//상하 입력 처리
	void LookUp(float value);
	//이동 속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float walkSpeed = 300;
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float runSpeed = 1200;
	//이동 방향
	FVector direction;
	//좌우 이동 입력
	void InputHorizontal(float value);
	//상하 이동 입력
	void InputVertical(float value);

	void InputDodgeRoll();			//구르기 입력
	bool isRollingAnim;				//구르기 애니메이션 재생중인가?
	bool isCoolTimeRolling;			//구르기 쿨타임중인가?
	float rollingCoolTime;			//구르기 쿨타임
	void CoolDownRolling();			//구르기 쿨타임 함수
	FTimerHandle RollingCoolTimerHandle;	//구르기 쿨 타이머 생성 및 관리
	FTimerHandle RollingAnimTimerHandle;	//구르기 애니메이션 재생 타이머 생성 및 관리
	FTimerHandle DashAttackAnimTimerHandle;	//대쉬공격 모션 재생 타이머 생성 및 관리

	void Move();					//이동 처리
	void InputRun();				//달리기
	//무기메시 컴포넌트 등록
	UPROPERTY(VisibleAnywhere, Category = WeaponMesh)
		class UStaticMeshComponent* weaponComp;

	//재생할 일반공격 몽타주
	//UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)		//bp_player에서 몽타주 삽입
	//	class UAnimMontage* NormalAttackMontage;
	int32 comboCnt;					//몇번째 콤보인지 확인변수
	bool isAttacking;				//공격중인가?
	bool isAttackButtonWhenAttack;	//공격중에 공격을 눌렀는가
	bool isDashAttacking;			//대쉬공격중인가?		(활성화 될 경우 isAttacking과 함께 활성화)
	void LMB_Click();				//일반 공격(마우스 왼쪽)을 눌렀을 때 함수
	void NormalAttack();			//일반 공격 함수
	void DashAttack();				//대쉬 공격 함수
	void RollingDelay();			//구르기 애니메이션 딜레이 함수
	void DashAttackDelay();			//대쉬공격 모션 딜레이 함수

	//노티파이 호출 함수
	UFUNCTION(BlueprintCallable)	//공격중에 한번 더 공격을 눌렀는지 확인 함수
		void AttackInputChecking();
	UFUNCTION(BlueprintCallable)	//공격이 끝났는지 확인 함수
		void EndAttacking();
};
