#include "MyGameStateBase.h"
#include "CoinActor.h"
#include "MyPressurePlate.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMyGameStateBase::AMyGameStateBase() {
	PrimaryActorTick.bCanEverTick = true;
	bPuzzleCompleted = false;
	
	PressurePlates.Empty();
	UpdateTotalCoinsInLevel();
}

void AMyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyGameStateBase, totalLevelCoins);
	DOREPLIFETIME(AMyGameStateBase, bPuzzleCompleted);
}

void AMyGameStateBase::UpdateTotalCoinsInLevel() {
	TArray<AActor*> foundCoins;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoinActor::StaticClass(), foundCoins);
	totalLevelCoins = foundCoins.Num();
}

void AMyGameStateBase::MulticastOnLevelComplete_Implementation(APawn* character, bool succeeded) {
	OnLevelCompleted(character, succeeded);
}

void AMyGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	// Find all pressure plates in the level
	FindPressurePlates();
}
void AMyGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Check puzzle state every frame on server
	if (HasAuthority())
		CheckPuzzleState();
}

void AMyGameStateBase::FindPressurePlates()
{
	// Automatically find all pressure plates in the level
	TArray<AActor*> FoundPlates;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyPressurePlate::StaticClass(), FoundPlates);
	PressurePlates.Empty();
	for (AActor* Plate : FoundPlates)
	{
		if (AMyPressurePlate* PressurePlate = Cast<AMyPressurePlate>(Plate))
		{
			PressurePlates.Add(PressurePlate);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("GameState found %d pressure plates for puzzle."), PressurePlates.Num());
}
void AMyGameStateBase::OnRep_PuzzleCompleted()
{
	// Called on clients when puzzle completion state changes
	// Here you could trigger visual/audio feedback for puzzle completion
}

void AMyGameStateBase::CheckPuzzleState()
{
	// Only run on server
	if (!HasAuthority())
		return;
	// Check if all pressure plates are activated
	bool bAllPlatesActive = true; // Start with true, then check if any are inactive
	if (PressurePlates.Num() > 0) {
		for (AMyPressurePlate* Plate : PressurePlates) {
			if (Plate && !Plate->IsActivated()) {
				bAllPlatesActive = false;
				break;
			}
		}
	}
	else
		bAllPlatesActive = true;

	bool bAllCoinsCollected = (totalLevelCoins == 0);
	bool bNewPuzzleState = bAllPlatesActive && bAllCoinsCollected;
	if (bNewPuzzleState != bPuzzleCompleted) {
		bPuzzleCompleted = bNewPuzzleState;
		OnRep_PuzzleCompleted();
	}
}
