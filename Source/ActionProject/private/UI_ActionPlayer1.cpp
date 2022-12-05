// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ActionPlayer1.h"
#include "ActionPlayer1.h"
#include "Player1AttackComponent.h"
#include "Player1MoveComponent.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include <Components/Overlay.h>
#include <Components/Image.h>

void UUI_ActionPlayer1::UpdateHealthBar()
{
	if (!OwnerPlayer.IsValid())
		return;

	HealthBar->SetPercent(OwnerPlayer->player1Health / OwnerPlayer->player1MaxHealth);

	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	CurrentHealthLabel->SetText(FText::AsNumber(OwnerPlayer->player1Health, &Opts));
	MaxHealthLabel->SetText(FText::AsNumber(OwnerPlayer->player1MaxHealth, &Opts));
}

void UUI_ActionPlayer1::UpdateSkill1CoolTime()
{
	if (!OwnerPlayer.IsValid())
		return;
	UPlayer1AttackComponent* attackVar = OwnerPlayer->FindComponentByClass<UPlayer1AttackComponent>();
	Skill1CoolTimeBar->SetPercent(attackVar->skill1CoolTime / attackVar->maxSkill1CoolTime);
}

void UUI_ActionPlayer1::UpdateSkill2CoolTime()
{
	if (!OwnerPlayer.IsValid())
		return;
	UPlayer1AttackComponent* attackVar = OwnerPlayer->FindComponentByClass<UPlayer1AttackComponent>();
	Skill2CoolTimeBar->SetPercent(attackVar->skill2CoolTime / attackVar->maxSkill2CoolTime);
}

void UUI_ActionPlayer1::UpdateSkill3CoolTime()
{
	if (!OwnerPlayer.IsValid())
		return;
	UPlayer1AttackComponent* attackVar = OwnerPlayer->FindComponentByClass<UPlayer1AttackComponent>();
	Skill3CoolTimeBar->SetPercent(attackVar->skill3CoolTime / attackVar->maxSkill3CoolTime);
}

void UUI_ActionPlayer1::UpdateSkill4CoolTime()
{
	if (!OwnerPlayer.IsValid())
		return;
	UPlayer1AttackComponent* attackVar = OwnerPlayer->FindComponentByClass<UPlayer1AttackComponent>();
	Skill4CoolTimeBar->SetPercent(attackVar->skill4CoolTime / attackVar->maxSkill4CoolTime);
}

void UUI_ActionPlayer1::UpdateUltCoolTime()
{
	if (!OwnerPlayer.IsValid())
		return;
	UPlayer1AttackComponent* attackVar = OwnerPlayer->FindComponentByClass<UPlayer1AttackComponent>();
	UltCoolTimeBar->SetPercent(attackVar->ultimateCoolTime / attackVar->maxUltimateCoolTime);
}

void UUI_ActionPlayer1::UpdateChargeBar()
{
	if (!OwnerPlayer.IsValid())
		return;
	UPlayer1AttackComponent* attackVar = OwnerPlayer->FindComponentByClass<UPlayer1AttackComponent>();
	ChargeBar->SetPercent(attackVar->curSkill4Charge / attackVar->maxSkill4Charge);
}

void UUI_ActionPlayer1::OnVisibilityChargeBar()
{
	ChargeBarOverLay->SetVisibility(ESlateVisibility::Visible);
	PerfectZone->SetVisibility(ESlateVisibility::Visible);	
}

void UUI_ActionPlayer1::OffVisibilityChargeBar()
{
	ChargeBarOverLay->SetVisibility(ESlateVisibility::Hidden);
	PerfectZone->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_ActionPlayer1::UpdateDodgeCoolTime()
{
	if (!OwnerPlayer.IsValid())
		return;
	UPlayer1MoveComponent* moveVar = OwnerPlayer->FindComponentByClass<UPlayer1MoveComponent>();
	DodgeCoolTimeBar->SetPercent(moveVar->rollingCoolTime / moveVar->maxRollingCoolTime);
}

void UUI_ActionPlayer1::VisibilityDodgeBar()
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
