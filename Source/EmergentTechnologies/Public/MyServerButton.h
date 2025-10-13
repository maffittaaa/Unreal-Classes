// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyMatchSessionInfo.h"
#include "Components/Button.h"
#include "MyServerButton.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API UMyServerButton : public UButton
{
	GENERATED_BODY()

	public:
		UMyServerButton();

		void SetSessionInfo(const FMatchSessionInfo& InInfo);

	private:
		FMatchSessionInfo SessionInfo;

		UFUNCTION()
		void OnClick();
	
};
