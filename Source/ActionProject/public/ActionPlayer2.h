// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayer.h"
#include "ActionPlayer2.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPROJECT_API AActionPlayer2 : public ABasePlayer
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionPlayer2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = HatMesh)
		class UStaticMeshComponent* hatComp;				//모자메시 컴포넌트 등록
	UPROPERTY(VisibleAnywhere, Category = WeaponMesh)
		class UStaticMeshComponent* weaponComp;				//무기메시 컴포넌트 등록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UArrowComponent* skillArrow;

	bool isAttacking = false;				//공격중인가?		(공격중엔 움직임 x, 구르기 o)
	
	//좌우 입력 처리
	void Turn(float value);
	//상하 입력 처리
	void LookUp(float value);

	//이동 속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float walkSpeed = 300;
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float runSpeed = 1200;

	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_NormalAttack> normalAttackFactory;			//일반공격액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_DashAttack> dashAttackFactory;			//대쉬공격액터생성팩토리

	void InputDodgeRoll();						//구르기 입력
	bool isRollingAnim = false;					//구르기 애니메이션 재생중인가?
	bool isCoolTimeRolling = false;				//구르기 쿨타임중인가?
	float maxRollingCoolTime = 5;				//최대 구르기 쿨타임 = 5초
	float rollingCoolTime = 0;					//구르기 쿨타임
	void CoolDownRolling();						//구르기 쿨타임 함수
	void DodgeEnd();							//구르기 애니메이션 완료 함수
	void LMB_Click();							//일반 공격(마우스 왼쪽)을 눌렀을 때 함수
	void NormalAttack();						//일반 공격 함수
	void DashAttack();							//대쉬 공격 함수

	//이동 방향
	FVector direction;
	//좌우 이동 입력
	void InputHorizontal(float value);
	//상하 이동 입력
	void InputVertical(float value);

	void Move();					//이동 처리
	void InputRun();				//달리기 Press
	void OutputRun();				//달리기 Release

	FTimerHandle RollingCoolTimerHandle;		//구르기 쿨 타이머 생성 및 관리

	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class AGhostTrail> ghostTrailDodgeFactory;	//구르기 잔상 액터생성팩토리


	//노티파이 호출 함수
	UFUNCTION(BlueprintCallable)	//구르기 잔상 생성 함수
		void CreateGhostTrail_Dodge();
	UFUNCTION(BlueprintCallable)	//구르기 애니메이션 종료 함수
		void Notify_DodgeEnd();
	UFUNCTION(BlueprintCallable)	//공격이 끝났는지 확인 함수
		void EndAttacking();
	UFUNCTION(BlueprintCallable)	//일반공격 이펙트 출력 함수
		void CreateNormalAttackEffect();
	UFUNCTION(BlueprintCallable)	//대쉬공격 이펙트 출력 함수
		void CreateDashAttackEffect();
};
