// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AEnemyAIController : public AAIController {
	GENERATED_BODY()

	public:
		AEnemyAIController();
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UAISenseConfig_Sight* sightConfig;

	protected:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UAIPerceptionComponent* aiPerceptionComponent;

		UFUNCTION()
		void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
		virtual void OnPossess(APawn* InPawn) override;
};



