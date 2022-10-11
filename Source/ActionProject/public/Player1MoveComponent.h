// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player1BaseComponent.h"
#include "Player1MoveComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPROJECT_API UPlayer1MoveComponent : public UPlayer1BaseComponent
{
	GENERATED_BODY()
	

public:
	UPlayer1MoveComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	//좌우 입력 처리
	void Turn(float value);
	//상하 입력 처리
	void LookUp(float value);

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	//이동 속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float walkSpeed = 300;
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float runSpeed = 1200;

	void InputDodgeRoll();						//구르기 입력
	bool isRollingAnim = false;					//구르기 애니메이션 재생중인가?
	bool isCoolTimeRolling = false;				//구르기 쿨타임중인가?
	float maxRollingCoolTime = 5;				//최대 구르기 쿨타임 = 5초
	float rollingCoolTime = 0;					//구르기 쿨타임
	void CoolDownRolling();						//구르기 쿨타임 함수
	void DodgeEnd();							//구르기 애니메이션 완료 함수

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
};
