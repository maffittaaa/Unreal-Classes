// Copyright Epic Games, Inc. All Rights Reserved.

#include "EmergentTechnologiesGameMode.h"

#include "EmergentTechnologiesCharacter.h"
#include "MyPlayerState.h"

AEmergentTechnologiesGameMode::AEmergentTechnologiesGameMode() {
	respawnDelay = 3.0f;
}

void AEmergentTechnologiesGameMode::RespawnPlayer(AController* controller) {
	if (!controller || !HasAuthority())
		return;

	UE_LOG(LogTemp, Warning, TEXT("respawnPlayer called for: %s"), *controller->GetName());

	APawn* oldPawn = controller->GetPawn();
	if (oldPawn) {
		controller->UnPossess();
		oldPawn->Destroy();
	}

	if (FTimerHandle* existingTimer = pendingRespawns.Find(controller))
		GetWorldTimerManager().ClearTimer(*existingTimer);

	FTimerHandle respawnTimer;
	FTimerDelegate respawnDelegate;
	respawnDelegate.BindUFunction(this, FName("RespawnPlayerInternal"), controller);

	GetWorldTimerManager().SetTimer(
		respawnTimer,
		respawnDelegate,
		respawnDelay,
		false);

	pendingRespawns.Add(controller, respawnTimer);
}

void AEmergentTechnologiesGameMode::RespawnPlayerInternal(AController* controller) {
	if (!controller) {
		UE_LOG(LogTemp, Warning, TEXT("RespawnPlayerInternal: controller is null"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("respawnPlayer called for: %s"), *controller->GetName());

	if (AMyPlayerState* myPlayerState = controller->GetPlayerState<AMyPlayerState>()) {}

	RestartPlayer(controller);
	pendingRespawns.Remove(controller);

	UE_LOG(LogTemp, Warning, TEXT("Player %s, respawned"), *controller->GetName());
}
