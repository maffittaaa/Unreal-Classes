// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "MyLaser.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AMyLaser : public AActor
{
	GENERATED_BODY()
	
	public:	
		// Sets default values for this actor's properties
		// UFUNCTION()
		// void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
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

	protected: 
		virtual void BeginPlay() override;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
		USphereComponent* collisionSphere;

	public:	
		// Called every frame
		virtual void Tick(float DeltaTime) override;
};
