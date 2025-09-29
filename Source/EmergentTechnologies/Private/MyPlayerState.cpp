// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

void AMyPlayerState::AddCoin()
{
	collectedCoins++;
	UE_LOG(LogTemp, Warning, TEXT("Total coins collected: %d"), collectedCoins);
}

