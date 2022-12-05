// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ActionPlayer2.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include <Components/Overlay.h>
#include <Components/Image.h>

void UUI_ActionPlayer2::UpdateHealthBar()
{
	if (!OwnerPlayer.IsValid())
		return;

	HealthBar->SetPercent(OwnerPlayer->playerHealth / OwnerPlayer->playerMaxHealth);

	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	CurrentHealthLabel->SetText(FText::AsNumber(OwnerPlayer->playerHealth, &Opts));
	MaxHealthLabel->SetText(FText::AsNumber(OwnerPlayer->playerMaxHealth, &Opts));
}

void UUI_ActionPlayer2::UpdateSkill1CoolTime()
{
	if (!OwnerPlayer.IsValid())
		return;
	Skill1CoolTimeBar->SetPercent(OwnerPlayer->skill1CoolTime / OwnerPlayer->maxSkill1CoolTime);
}

void UUI_ActionPlayer2::UpdateSkill2CoolTime()
{
	if (!OwnerPlayer.IsValid())
		return;
	Skill2CoolTimeBar->SetPercent(OwnerPlayer->skill2CoolTime / OwnerPlayer->maxSkill2CoolTime);
}

void UUI_ActionPlayer2::UpdateSkill3CoolTime()
{
	if (!OwnerPlayer.IsValid())
		return;
	Skill3CoolTimeBar->SetPercent(OwnerPlayer->skill3CoolTime / OwnerPlayer->maxSkill3CoolTime);
}

void UUI_ActionPlayer2::UpdateSkill4CoolTime()
{
	if (!OwnerPlayer.IsValid())
		return;
	Skill4CoolTimeBar->SetPercent(OwnerPlayer->skill4CoolTime / OwnerPlayer->maxSkill4CoolTime);
}

void UUI_ActionPlayer2::UpdateUltCoolTime()
{
	if (!OwnerPlayer.IsValid())
		return;
	UltCoolTimeBar->SetPercent(OwnerPlayer->ultimateCoolTime / OwnerPlayer->maxUltimateCoolTime);
}

void UUI_ActionPlayer2::UpdateChargeBar()
{
	if (!OwnerPlayer.IsValid())
		return;
	ChargeBar->SetPercent(OwnerPlayer->curSkill3Charge / OwnerPlayer->maxSkill3Charge);
}

void UUI_ActionPlayer2::OnVisibilityChargeBar()
{
	ChargeBarOverLay->SetVisibility(ESlateVisibility::Visible);
}

void UUI_ActionPlayer2::OffVisibilityChargeBar()
{
	ChargeBarOverLay->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_ActionPlayer2::UpdateDodgeCoolTime()
{
	if (!OwnerPlayer.IsValid())
		return;
	DodgeCoolTimeBar->SetPercent(OwnerPlayer->rollingCoolTime / OwnerPlayer->maxRollingCoolTime);
}

void UUI_ActionPlayer2::VisibilityDodgeBar()
{
	if (DodgeCoolTimeBar->GetVisibility() == ESlateVisibility::Hidden)
	{
		DodgeCoolTimeBar->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		DodgeCoolTimeBar->SetVisibility(ESlateVisibility::Hidden);
	}
}
