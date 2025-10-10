// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AIController.h"
#include "MyInterface.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AEnemyCharacter : public ACharacter, public IMyInterface
{
	GENERATED_BODY()

	public:
		// Sets default values for this character's properties
		AEnemyCharacter();

		UFUNCTION()
		void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus stimulus);

		// virtual void TakeDamageFromObject_Implementation(float damageAmount, AActor* damageCauser) override;

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
		TArray<AActor*> waypoints;
		AAIController* myAIController;
		ATargetPoint* GetRandomWaypoint();
		void AIMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result);

		AActor* target;

		//replicated properties for movement and animation
		UPROPERTY(ReplicatedUsing = OnRep_CurrentSpeed)
		float currentSpeed;

		UPROPERTY(ReplicatedUsing = OnRep_AnimationRate)
		float animationRate;

		UFUNCTION()
		void OnRep_CurrentSpeed();

		UFUNCTION()
		void OnRep_AnimationRate();

		void SetEnemySpeed(float newSpeed, float newAnimRate);
		
	public:	
		// Called every frame
		virtual void Tick(float DeltaTime) override;

		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
