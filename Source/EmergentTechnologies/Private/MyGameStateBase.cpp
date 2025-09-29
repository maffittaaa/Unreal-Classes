#include "MyGameStateBase.h"
#include "CoinActor.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMyGameStateBase::AMyGameStateBase() {
	UpdateTotalCoinsInLevel();
}

void AMyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyGameStateBase, totalLevelCoins);
}

void AMyGameStateBase::UpdateTotalCoinsInLevel() {
	TArray<AActor*> foundCoins;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoinActor::StaticClass(), foundCoins);
	totalLevelCoins = foundCoins.Num();
}