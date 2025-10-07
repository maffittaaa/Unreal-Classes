// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyHealth.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AMyHealth : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void InitializeHealth(float newMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamageFromObject(float damageAmount);
	

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const { return bIsDead; }
	
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth() const { return currentHealth; }

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const { return maxHealth; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	bool bIsDead;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
