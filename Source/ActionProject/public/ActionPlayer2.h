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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UArrowComponent* lineTracingArrow;
	UPROPERTY(VisibleAnywhere, Category = Decal)
		class UDecalComponent* skill2Area;				//스킬 2 범위

	bool isImpacting = false;				//피격중인가?		(조작 x)
	bool isAttacking = false;				//공격중인가?		(공격중엔 움직임 x, 구르기 o)
	bool isSkill3Attacking = false;			//스킬3공격중인가?	(공격중엔 움직임 x, 구르기 o, 시선회전 x)
	bool isUltimateAttacking = false;		//궁극기중인가?		(공격중엔 움직임 x, 구르기 x, 시선회전 x)

	bool isCoolTimeSkill1 = false;			//스킬1 쿨타임중인가?
	float skill1CoolTime = 0;			//현재 스킬1 쿨타임
	float maxSkill1CoolTime = 5;		//스킬1 쿨타임

	bool isCoolTimeSkill2 = false;			//스킬2 쿨타임중인가?
	float skill2CoolTime = 0;			//현재스킬2 쿨타임
	float maxSkill2CoolTime = 15;		//스킬2 쿨타임

	bool isCoolTimeSkill3 = false;			//스킬3 쿨타임중인가?
	float skill3CoolTime = 0;			//현재스킬3 쿨타임
	float maxSkill3CoolTime = 15;		//스킬3 쿨타임
	float maxSkill3Charge = 4.0f;				//스킬2 총 차징시간
	float curSkill3Charge = 0;				//스킬2 현재 차징시간

	bool isCoolTimeSkill4 = false;			//스킬4 쿨타임중인가?
	float skill4CoolTime = 0;			//현재 스킬4 쿨타임
	float maxSkill4CoolTime = 10;		//스킬4 쿨타임

	bool isCoolTimeUltimate = false;		//궁극기 쿨타임중인가?
	float ultimateCoolTime = 10;			//현재 궁극기 쿨타임
	float maxUltimateCoolTime = 60;		//궁극기 쿨타임
	
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
		TSubclassOf<class APlayer2_NormalAttack> normalAttackFactory;		//일반공격액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_DashAttack> dashAttackFactory;			//대쉬공격액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_Skill1> skill1AttackFactory;				//스킬1액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_Skill2_Portal> skill2AttackFactory;		//스킬2액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_Skill3> skill3AttackFactory;				//스킬3액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_Skill4Factory> skill4AttackFactory;		//스킬4액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer2_UltimateFactory> ultTornadoAttackFactory;	//궁극기 토네이도 액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> HPBarWidget;

	class UUI_ActionPlayer2* Widget;

	void InputDodgeRoll();						//구르기 입력
	bool isRollingAnim = false;					//구르기 애니메이션 재생중인가?
	bool isCoolTimeRolling = false;				//구르기 쿨타임중인가?
	float maxRollingCoolTime = 5;				//최대 구르기 쿨타임 = 5초
	float rollingCoolTime = 0;					//구르기 쿨타임
	void CoolDownRolling();						//구르기 쿨타임 함수
	void DodgeEnd();							//구르기 애니메이션 완료 함수
	void LMB_Click();							//일반 공격(마우스 왼쪽)을 눌렀을 때 함수
	void RMB_Click();							//마우스 오른쪽 클릭
	void NormalAttack();						//일반 공격 함수
	void DashAttack();							//대쉬 공격 함수
	void Skill1Attack();						//스킬1 공격 함수
	void CoolDownSkill1();						//스킬1 쿨타임 함수
	void Skill2Area();							//스킬2 공격 범위
	void Skill2Attack();						//스킬2 공격 함수
	void CoolDownSkill2();						//스킬2쿨타임 함수
	void Skill3Attack();						//스킬3 공격 함수
	void CoolDownSkill3();						//스킬3 쿨타임 함수
	void Skill3End();							//스킬3 차지 종료 함수
	void Skill4Attack();						//스킬4 공격 함수
	void CoolDownSkill4();						//스킬4 쿨타임 함수
	void UltimateAttack();						//궁극기 공격 함수
	void CoolDownUltimate();					//궁극기 쿨타임 함수
	void UltimateHoldOff();						//궁극기 자세 유지 해제
	void PlayerDie();							//플레이어 사망 함수
	void GamePause();							//플레이어 사망 애니메이션 이후 게임 정지
	bool turnskill2Area = false;				//스킬2 범위를 킨 상태인가?
	FHitResult HitResult;						//라인트레이스 도착지점

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
	FTimerHandle Skill3EndHandle;				//스킬3 차지타이머
	FTimerHandle UltimateHoldOffHandle;			//궁극기 자세 해제 관리

	FTimerHandle Skill1CoolTimerHandle;			//스킬1 쿨 타이머 생성 및 관리
	FTimerHandle Skill2CoolTimerHandle;			//스킬2 쿨 타이머 생성 및 관리
	FTimerHandle Skill3CoolTimerHandle;			//스킬3 쿨 타이머 생성 및 관리
	FTimerHandle Skill4CoolTimerHandle;			//스킬4 쿨 타이머 생성 및 관리
	FTimerHandle UltimateCoolTimerHandle;		//궁극기 쿨 타이머 생성 및 관리
	FTimerHandle DieDelayTimerHandle;			//사망 애니메이션 딜레이 관리

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
	UFUNCTION(BlueprintCallable)	//스킬1 이펙트 출력 함수
		void CreateSkill1AttackEffect();
	UFUNCTION(BlueprintCallable)	//스킬3 이펙트 출력 함수
		void CreateSkill3AttackEffect();
	UFUNCTION(BlueprintCallable)	//스킬4 이펙트 출력 함수
		void CreateSkill4AttackEffect();
	UFUNCTION(BlueprintCallable)	//궁극기 이펙트 출력 함수
		void CreateUltimateAttackEffect();
	UFUNCTION(BlueprintCallable)	//피격 모션 종료 함수
		void SmallImpactEnd();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
