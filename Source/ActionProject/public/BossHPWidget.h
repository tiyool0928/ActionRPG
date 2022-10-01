// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyBoss.h"
#include "BossHPWidget.generated.h"

/**
 * 
 */
UCLASS( Abstract )
class ACTIONPROJECT_API UBossHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	TWeakObjectPtr<AEnemyBoss> OwnerBoss;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentHealthLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxHealthLabel;

public:
	void UpdateHealthBar();
	void SetOwnerBoss(AEnemyBoss* InBoss) { OwnerBoss = InBoss; }
};