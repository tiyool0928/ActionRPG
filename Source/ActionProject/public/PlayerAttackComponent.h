// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerAttackComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPROJECT_API UPlayerAttackComponent : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerAttackComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	bool canDamage;					//콤보공격 유효 타이밍
	int comboCnt;					//몇번째 콤보인지 확인변수

	void NormalAttack();			//일반 공격 함수
	void DashAttack();				//대쉬 공격 함수
	void DashAttackDelay();			//대쉬공격 모션 딜레이 함수
	void InputSkill1();				//스킬1 함수
	void CoolDownSkill1();			//스킬1 쿨타임 함수
	void InputSkill2();				//스킬2 함수
	void CoolDownSkill2();			//스킬2 쿨타임 함수
	void InputSkill3();				//스킬3 함수
	void CoolDownSkill3();			//스킬3 쿨타임 함수
	void InputSkill4();				//스킬4 함수
	void OutputSkill4();			//스킬4 함수
	void Skill4FeverOnDelay();		//스킬4 강화공격 딜레이 함수
	void Skill4FeverOffDelay();		//스킬4 강화공격 피버 종료 함수
	void Skill4EndDelay();			//스킬4 애니메이션 딜레이 함수
	void Skill4EndMotionDelay();	//스킬4 종료 모션 딜레이 함수
	void CoolDownSkill4();			//스킬4 쿨타임 함수
	void InputUltimate();			//궁극기 함수
	void CoolDownUltimate();		//궁극기 쿨타임 함수


	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class AGhostTrail_Skill> ghostTrailSkillFactory;	//스킬 잔상 액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer1_Skill1> skill1Factory;				//스킬 1액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer1_Skill3> skill3Factory;				//스킬 3액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer1_Skill4> skill4Factory;	//스킬 4액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer1_Skill4Landing> skill4LandingFactory;	//스킬 4액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class APlayer1_UltimateBoom> ultimateFactory;	//궁극기 액터생성팩토리

	FTimerHandle DashAttackAnimTimerHandle;		//대쉬공격 모션 재생 타이머 생성 및 관리
	FTimerHandle Skill1CoolTimerHandle;			//스킬1 쿨 타이머 생성 및 관리
	FTimerHandle Skill2CoolTimerHandle;			//스킬2 쿨 타이머 생성 및 관리
	FTimerHandle Skill3CoolTimerHandle;			//스킬3 쿨 타이머 생성 및 관리
	FTimerHandle Skill4FeverOnHandle;			//스킬4 강화공격 딜레이 관리
	FTimerHandle Skill4FeverOffHandle;			//스킬4 강화공격 피버 종료 딜레이 관리
	FTimerHandle Skill4EndDelayHandle;			//스킬4 애니메이션 딜레이 관리
	FTimerHandle Skill4EndMotionDelayHandle;	//스킬4 종료 모션 딜레이 관리
	FTimerHandle Skill4CoolTimerHandle;			//스킬4 쿨 타이머 생성 및 관리
	FTimerHandle UltimateCoolTimerHandle;		//궁극기 쿨 타이머 생성 및 관리

	void AttackDamageApplyingComp();	//공격의 데미지 영역 부분인지 확인 함수
	void AttackDamageEndComp();			//공격의 데미지 영역 부분이 끝나는 함수
	void CreateSkill1EffectComp();		//스킬1 이펙트 출력 함수
	void CreateSkill3EffectComp();		//스킬3 이펙트 출력 함수
	void Skill4CanDodgeComp();
};
