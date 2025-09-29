// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

#include "Net/UnrealNetwork.h"

void AMyPlayerState::AddCoin()
{
	collectedCoins++;
	UE_LOG(LogTemp, Warning, TEXT("Total coins collected: %d"), collectedCoins);
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyPlayerState, collectedCoins);
}