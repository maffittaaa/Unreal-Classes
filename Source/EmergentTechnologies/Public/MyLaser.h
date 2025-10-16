// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "EmergentTechnologiesCharacter.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "MyLaser.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AMyLaser : public AActor
{
	GENERATED_BODY()
	
	public:
		AMyLaser();

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* laserMesh;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UNiagaraComponent* niagaraLaser;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UNiagaraComponent* niagaraLaserImpact;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UNiagaraSystem* niagaraLaserSystem;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UNiagaraSystem* niagaraLaserImpactSystem;
	
		UFUNCTION(BlueprintCallable, Category = "Laser")
		void SetLaserColors();

		void TouchingLaser(AEmergentTechnologiesCharacter* projectCharacter);

	protected: 
		virtual void BeginPlay() override;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
		USphereComponent* collisionSphere;

	public:	
		// Called every frame
		virtual void Tick(float DeltaTime) override;
};
