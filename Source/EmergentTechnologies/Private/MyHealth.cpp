#include "MyHealth.h"

AMyHealth::AMyHealth() {
	PrimaryActorTick.bCanEverTick = false;
	maxHealth = 100.0f;
	currentHealth = maxHealth;
	bIsDead = false;
}

void AMyHealth::BeginPlay() {
	Super::BeginPlay();
	currentHealth = maxHealth;
}

void AMyHealth::InitializeHealth(float newMaxHealth) {
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), newMaxHealth);
	maxHealth = newMaxHealth;
	currentHealth = maxHealth;
	bIsDead = false;
}

void AMyHealth::TakeDamageFromObject(float damageAmount) {
	if (bIsDead || damageAmount <= 0.0f)
		return;

	currentHealth = FMath::Clamp(currentHealth - damageAmount, 0, maxHealth);
	if (currentHealth <= 0.0f)
		bIsDead = true;
}

void AMyHealth::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

