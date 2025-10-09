// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EmergentTechnologiesGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AEmergentTechnologiesGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:
		AEmergentTechnologiesGameMode();

		UFUNCTION()
		void RespawnPlayer(AController* controller);

	protected:
		UPROPERTY(EditDefaultsOnly, Category = "Respawn")
		float respawnDelay = 3.0f;

		TMap<AController*, FTimerHandle> pendingRespawns;

		UFUNCTION()
		void RespawnPlayerInternal(AController* controller);
};



