#include "MyDoor.h"
#include "Net/UnrealNetwork.h"

AMyDoor::AMyDoor() {
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	doorMesh->SetupAttachment(RootComponent);
	doorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	bIsActivated = false;
}

void AMyDoor::BeginPlay() {
	Super::BeginPlay();

	myGameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
}

void AMyDoor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMyDoor::OnRep_IsActivated() {
	myGameState->UpdateTotalDoorsActivated();
	UpdateInitialDoorVisuals();
	UpdateFinalDoorVisuals();
}

void AMyDoor::ActivateDoor() {
	bIsActivated = true;
	myGameState->UpdateTotalDoorsActivated();
	UE_LOG(LogTemp, Warning, TEXT("Door is activated!!"));
	OnRep_IsActivated();
}

void AMyDoor::DeactivateDoor() {
	bIsActivated = false;
	myGameState->UpdateTotalDoorsActivated();
	UE_LOG(LogTemp, Warning, TEXT("Door is deactivated!!"));
	OnRep_IsActivated();
}

void AMyDoor::UpdateInitialDoorVisuals() {
	if (RootComponent) {
		FVector CurrentLocation = RootComponent->GetComponentLocation();
		float TargetZ = bIsActivated ? 120.0f: 0.0f;
		RootComponent->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, TargetZ));
	}
}

void AMyDoor::UpdateFinalDoorVisuals() {
	FVector CurrentLocation = RootComponent->GetComponentLocation();
	float TargetZ;
	if (myGameState->totalMiniGameDoorsDeactivated == 1)
		TargetZ = 240.0f;
	else if (bIsActivated)
		TargetZ = 120.0f;
	else
		TargetZ = 0.0f;
	RootComponent->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, TargetZ));
	UE_LOG(LogTemp, Warning, TEXT(" Total Active: %d"), myGameState->totalMiniGameDoorsDeactivated);
}

void AMyDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyDoor, bIsActivated);
}

