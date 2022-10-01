// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHPWidget.h"
#include "EnemyBoss.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void UBossHPWidget::UpdateHealthBar()
{
	if (!OwnerBoss.IsValid())
		return;

	HealthBar->SetPercent(OwnerBoss->health / OwnerBoss->maxHealth);

	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	CurrentHealthLabel->SetText(FText::AsNumber(OwnerBoss->health, &Opts));
	MaxHealthLabel->SetText(FText::AsNumber(OwnerBoss->maxHealth, &Opts));
}
