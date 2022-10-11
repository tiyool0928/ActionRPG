// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionPlayer1.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Player1BaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONPROJECT_API UPlayer1BaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayer1BaseComponent();

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
