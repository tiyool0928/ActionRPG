// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionPlayer1.h"
#include "UI_ActionPlayer1.generated.h"

/**
 * 
 */
UCLASS( Abstract )
class ACTIONPROJECT_API UUI_ActionPlayer1 : public UUserWidget
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<AActionPlayer1> OwnerPlayer;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentHealthLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxHealthLabel;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Skill1CoolTimeBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Skill2CoolTimeBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Skill3CoolTimeBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Skill4CoolTimeBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ChargeBar;
	UPROPERTY(meta = (BindWidget))
		class UOverlay* ChargeBarOverLay;
	UPROPERTY(meta = (BindWidget))
		class UImage* PerfectZone;

public:
	void UpdateHealthBar();
	void UpdateSkill1CoolTime();
	void UpdateSkill2CoolTime();
	void UpdateSkill3CoolTime();
	void UpdateSkill4CoolTime();
	void UpdateChargeBar();
	void VisibilityChargeBar();
	void SetOwnerPlayer(AActionPlayer1* InPlayer) { OwnerPlayer = InPlayer; }
};
