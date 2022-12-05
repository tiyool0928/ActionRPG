// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionPlayer2.h"
#include "UI_ActionPlayer2.generated.h"

/**
 * 
 */
UCLASS( Abstract )
class ACTIONPROJECT_API UUI_ActionPlayer2 : public UUserWidget
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<AActionPlayer2> OwnerPlayer;

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
		class UProgressBar* UltCoolTimeBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ChargeBar;
	UPROPERTY(meta = (BindWidget))
		class UOverlay* ChargeBarOverLay;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* DodgeCoolTimeBar;

public:
	void UpdateHealthBar();
	void UpdateSkill1CoolTime();
	void UpdateSkill2CoolTime();
	void UpdateSkill3CoolTime();
	void UpdateSkill4CoolTime();
	void UpdateUltCoolTime();
	void UpdateChargeBar();
	void OnVisibilityChargeBar();
	void OffVisibilityChargeBar();
	void UpdateDodgeCoolTime();
	void VisibilityDodgeBar();
	void SetOwnerPlayer(AActionPlayer2* InPlayer) { OwnerPlayer = InPlayer; }
};
