// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionPlayer1.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONPROJECT_API UPlayerBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerBaseComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) {};

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
		AActionPlayer1* me;

	UPROPERTY()
		UCharacterMovementComponent* moveComp;
};
