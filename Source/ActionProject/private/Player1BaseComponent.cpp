// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1BaseComponent.h"

// Sets default values for this component's properties
UPlayer1BaseComponent::UPlayer1BaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayer1BaseComponent::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<AActionPlayer1>(GetOwner());
	moveComp = me->GetCharacterMovement();
}


// Called every frame
void UPlayer1BaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
