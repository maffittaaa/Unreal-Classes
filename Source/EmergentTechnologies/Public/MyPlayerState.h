// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	public:
		UFUNCTION()
		void AddCoin();
	
	private:
		UPROPERTY(Replicated, VisibleAnywhere, Category = "Coins")
		int collectedCoins;
};