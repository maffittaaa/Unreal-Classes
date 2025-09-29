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
	
};
