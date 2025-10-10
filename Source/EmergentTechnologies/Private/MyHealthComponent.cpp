#include "MyHealthComponent.h"

UMyHealthComponent::UMyHealthComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	maxHealth = 100.0f;
	currentHealth = maxHealth;
	bIsDead = false;
}

void UMyHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	currentHealth = maxHealth;
}

void UMyHealthComponent::InitializeHealth(float newMaxHealth) {
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), newMaxHealth);
	maxHealth = newMaxHealth;
	currentHealth = maxHealth;
	bIsDead = false;
}

void UMyHealthComponent::TakeDamageFromObject(float damageAmount) {
	if (bIsDead || damageAmount <= 0.0f)
		return;
	
	currentHealth = FMath::Clamp(currentHealth - damageAmount, 0, maxHealth);
	OnHealthChanged.Broadcast(currentHealth);
	UE_LOG(LogTemp, Warning, TEXT("Current health after damage: %f"), currentHealth );
	
	if (currentHealth <= 0.0f) {
		bIsDead = true;
		OnDeath.Broadcast();
	}
}

void UMyHealthComponent::HealFromObject(float healAmount) {
	if (bIsDead || healAmount > 100.0f)
		return;
	
	currentHealth = FMath::Clamp(currentHealth + healAmount, 0, maxHealth);
	OnHealthChanged.Broadcast(currentHealth);
}

void UMyHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

