// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "CoinActor.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API ACoinActor : public AActor
{
	GENERATED_BODY()
	
	public:	
		// Sets default values for this actor's properties
		UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		ACoinActor();
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* coinMesh;
	
		
	protected:
		virtual void BeginPlay() override;
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
		USphereComponent* collisionSphere; //point to an object

		UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UParticleSystem* collectEffects;

		UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UParticleSystemComponent* effect;

		UFUNCTION(NetMulticast, Reliable)
		void PlayEffects();

public:	
		// Called every frame
		virtual void Tick(float DeltaTime) override;
};
