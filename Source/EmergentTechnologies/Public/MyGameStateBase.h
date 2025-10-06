#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	public:
		AMyGameStateBase();
		void UpdateTotalCoinsInLevel();

		UPROPERTY(Replicated, BlueprintReadOnly)
		int totalLevelCoins;

		UFUNCTION(NetMulticast, Reliable)
		void MulticastOnLevelComplete(APawn* character, bool succeeded);

		UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay Events")
		void OnLevelCompleted(APawn* character, bool succeeded);
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle")
		TArray<class AMyPressurePlate*> PressurePlates;

		UPROPERTY(ReplicatedUsing=OnRep_PuzzleCompleted, VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle")
		bool bPuzzleCompleted;

		UFUNCTION(BlueprintPure, Category = "Puzzle")
		bool IsPuzzleComplete() const { return bPuzzleCompleted; }

	protected:
		UFUNCTION()
		void OnRep_PuzzleCompleted();

		void CheckPuzzleState();

		void FindPressurePlates();

	public:
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
};
