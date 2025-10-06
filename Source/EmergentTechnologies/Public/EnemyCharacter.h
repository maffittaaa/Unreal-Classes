// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<AActor*> waypoints;
	AAIController* myAIController;
	ATargetPoint* GetRandomWaypoint();
	void AIMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result);
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UAIPerceptionComponent* aiPerceptionComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UAISenseConfig_Sight* sightConfig;

	AActor* target;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus stimulus);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
